#
# Let's quickly clear out what this is and isn't.
# This is not a Nix package flake for Artificial Threat project (yet?).
# This IS a flake for the "nix develop" command.
#
{
  description = "Artificial Threat development environment";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = inputs@{ self, nixpkgs, ... }: let
    system = "x86_64-linux";
    mingwGameLibsOverlay = final: prev: {
      allegro = prev.allegro.overrideAttrs (oldAttrs: {
        meta = (oldAttrs.meta or {}) // {
          platforms = final.lib.platforms.all; # Bypass platform constraint check
        };

        doCheck = false;
        checkInputs = [ ];
        buildInputs = [ ];
        nativeBuildInputs = [ final.stdenv.cc ];

        configureFlags = [
          "--host=${final.stdenv.hostPlatform.config}"
          "--enable-static"
          "--disable-shared"
          "--disable-x11"
          "--disable-alsadigi"
          "--disable-alsamidi"
        ];
      });
      libopenmpt = prev.libopenmpt.overrideAttrs (oldAttrs: {
        meta = (oldAttrs.meta or {}) // {
          platforms = final.lib.platforms.all;
        };

        # LGTM :3 (clueless)
        doCheck = false;
        checkInputs = [ ];
        nativeBuildInputs = [ final.stdenv.cc ];
        buildInputs = [ ];
        propagatedBuildInputs = [ ];

        # ООО "Костыли и велосипеды"
        dontConfigure = true;
        buildPhase = ''
          runHook preBuild
          make -f Makefile.gnumake \
            CC="${final.stdenv.cc.targetPrefix}gcc" \
            CXX="${final.stdenv.cc.targetPrefix}g++" \
            AR="${final.stdenv.cc.targetPrefix}ar" \
            LD="${final.stdenv.cc.targetPrefix}g++" \
            CONFIG=mingw-w64 \
            NO_PULSEAUDIO=1 \
            NO_ALSA=1 \
            NO_PORTAUDIO=1 \
            NO_SNDFILE=1 \
            NO_XMP=1 \
            NO_FLAC=1 \
            NO_MINIMP3=1 \
            NO_VORBIS=1
          runHook postBuild
        '';
        installPhase = ''
          runHook preInstall
          mkdir -p $out/lib $out/include/libopenmpt
          
          # Copy cross-compiled static/dynamic library assets
          cp -v bin/*.a bin/*.dll $out/lib/ 2>/dev/null || cp -v bin/*.a $out/lib/
          
          # Extract internal C/C++ tracker headers
          cp -v libopenmpt/libopenmpt.h libopenmpt/libopenmpt_stream_callbacks_file.h $out/include/libopenmpt/
          cp -v libopenmpt/libopenmpt.hpp $out/include/libopenmpt/
          runHook postInstall
        '';

        dontStrip = true;
        outputs = [ "out" ]; 
      });
    };
    pkgs = import nixpkgs {
      inherit system;
      config.allowUnfree = true; # We're not running Debian here, are we?
    };
    pkgsWin32 = import nixpkgs {
      inherit system;
      config.allowUnfree = true; # We're not running Debian here, are we?
      config.microsoftVisualStudioLicenseAccepted = true;
      overlays = [ mingwGameLibsOverlay ];
      crossSystem = "i686-windows";
    };
  in {
    devShells.${system} = {
      default = pkgs.mkShell {
        packages = with pkgs; [
          gcc
          gnumake
          gdb # we'd need that
          libopenmpt
          allegro	# Yup, it's 4.4 and not 4.2, so it would have (some) differences compared to a DOS-compatible Allegro version.
          aseprite # Required now as the spritesheets are being generated from .ase files during game compilation
        ];
      };
      win32 = pkgs.mkShell {
        nativeBuildInputs = with pkgsWin32.buildPackages; [
          gcc
          gnumake
          gdb
          allegro # For DATFILE utilities
          aseprite
        ];
        buildInputs = with pkgsWin32; [
          libopenmpt
          allegro
        ];
      };
    };
  };
}
