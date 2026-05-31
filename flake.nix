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
    pkgs = import nixpkgs {
      inherit system;
      config.allowUnfree = true; # We're not running Debian here, are we?
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
          lua
          aseprite # Required now as the spritesheets are being generated from .ase files during game compilation
        ];
      };
    };
  };
}
