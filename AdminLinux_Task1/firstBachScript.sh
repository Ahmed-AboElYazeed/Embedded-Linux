#!/bin/bash
cd 
mkdir -p myDirectory
mkdir -p myDirectory/secondDirectory
touch myDirectory/secondDirectory/myNotePaper
cp myDirectory/secondDirectory/myNotePaper myDirectory/ 
mv myDirectory/myNotePaper myDirectory/myOldNotePaper 
