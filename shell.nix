{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = with pkgs; [
    gnumake
    # cmake

    clang
    llvm
    pkg-config

    SDL2
    SDL2_image
    SDL2_ttf
    SDL2_mixer
  ];
}