# Nix shell script. Used by the nix package manager to load all libraries that the project uses.
{ pkgs ? import <nixpkgs> {} } :
  pkgs.mkShell {
    packages = with pkgs; [
      #gnat13 # gcc and utils
      clang # Version 17
      clang-tools
      cmake
      gnumake
      bashInteractiveFHS
    ];
} 
