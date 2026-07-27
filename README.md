# liftspot-data-tool

Companion tool for [LiftSpot](https://github.com/selinmutlu06/liftspot) ; validates and cleans the elevator dataset before it ships to production.

A C++ CLI tool that validates and enriches the elevator dataset behind LiftSpot (liftspot.netlify.app), a map based elevator directory.

## What it does

1. Reads elevator records from a CSV (id, name, building, address, lat, lng)
2. Validates each record and flags missing/duplicate ids, missing/out-of-range/duplicate coordinates, missing or malformed addresses, missing building names
3. Enriches the data: finds each elevator's nearest neighbor by haversine distance, clusters elevators using union-find
4. Writes a cleaned CSV plus a plain text validation report

## Build

g++ -std=c++17 -O2 -o liftspot_data_tool src/main.cpp src/csv_utils.cpp src/geo_utils.cpp src/validator.cpp src/enrich.cpp

## Usage

./liftspot_data_tool <input.csv> <output.csv> [cluster_threshold_meters]
