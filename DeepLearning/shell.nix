{ pkgs ? import <nixpkgs> {} }:
let
  python-packages = ps: with ps; [
    stdenv.cc.cc.lib
    matplotlib
    jupyter
    notebook
    tensorflow
    keras
  ];
  python-deep-learning = pkgs.python3.withPackages python-packages;
in python-deep-learning.env
