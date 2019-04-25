#!/bin/bash

rsync  -av --exclude .git --exclude build --exclude doc $PWD debian@bbb:./

