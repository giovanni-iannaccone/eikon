#!/bin/bash

DEFAULT="\e[0;0m"
RED="\e[1;31m"

cli() {
    cd cli
    mkdir -p build
    cd build
    cmake ..
    make install && mv ../bin/eikoncli ../../bin
}

install() {
    mkdir -p build
    cd build
    cmake ..
    make install
}

print_usage() {
    echo "$0 <action>"
    echo
    echo " -i | --install       Install eikon on your system"
    echo " -c | --cli           Install eikon cli"
    echo " -t | --test          Compile tests for eikon"
    echo " -h | --help          Show this help message"
}

test() {
    cd test
    mkdir -p build
    cd build
    cmake ..
    make install && mv ../bin/eikon_test ../../bin
}

if [ $? -ne 0 ]; then
    print_usage
    exit 1
fi

if [[ $EUID -ne 0 ]]; then
   echo -e "$RED Need root priviledges to run this script $DEFAULT"
   exit 1
fi

while [ $# -gt 0 ]; do
    case $1 in
        -i | --install)
            install
            ;;
        -c | --cli)
            cli
            ;;
        -t | --test)
            test
            ;;
        -h | --help)
            print_usage
            exit 0
            ;;
        --)
            shift
            break
            ;;
        *)
            echo -e "$RED Invalid option: $1 $DEFAULT"
            print_usage
            exit 1
    esac
    shift
done
