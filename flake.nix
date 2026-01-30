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
    pkgs = nixpkgs.legacyPackages.${system};
  in {
    devShells.${system} = {
      default = pkgs.mkShell {
        packages = with pkgs; [
          gcc
          gnumake
          libopenmpt
          allegro	# Yup, it's 4.4 and not 4.2, so it would have (some) differences compared to a DOS-compatible Allegro version.
          lua
        ];
      };
    };
  };
}
