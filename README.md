# init

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![version](https://img.shields.io/github/v/release/albin-johansson/centurion)](https://github.com/albin-johansson/centurion/releases)
[![CI: Windows](https://github.com/albin-johansson/init/actions/workflows/windows.yml/badge.svg?branch=dev)](https://github.com/albin-johansson/init/actions/workflows/windows.yml)

A simple .ini parsing and serialization library, written in C++20.

## Features

* Written in modern C++20
* No dependencies other than the STL
* Header only
* Supports both parsing and writing ini files
* Support for custom syntax

## Installation

The library is header-only, include the headers in the `src` directory in your project and you're ready to go!

## Basic Usage

```C++
#include <init/ini.hpp>

void read()
{
  const auto ini = init::read_ini("foo.ini");  
  
  const auto& section = ini.at("section");
  
  int abc{};
  section.at("abc").get_to(abc);
  
  if (section.contains("def"))
  {
    auto def = section.at("def").as<float>();
  }
}

void write()
{
  init::ini ini;
  
  ini["section"]["abc"] = 42;
  ini["section"]["def"] = 12.3;
  
  init::write_ini(ini, "foo.ini");
}
```
