#!/bin/bash
FAIL=0

# Function to run a single test of the calc10 program.
testCalc10() {
  TESTNO=$1
  ESTATUS=$2

  rm -f output.txt
  
  echo "Test $TESTNO: ./calc10 < input-10-$TESTNO.txt > output.txt"
  ./calc10 < input-10-$TESTNO.txt > output.txt
  STATUS=$?

  # Make sure the program exited with the right exit status.
  if [ $STATUS -ne $ESTATUS ]; then
      echo "**** FAILED - Expected an exit status of $ESTATUS, but got: $STATUS"
      FAIL=1
      return 1
  fi

  # Make sure output matches expected output.
  if ! diff -q expected-10-$TESTNO.txt output.txt >/dev/null 2>&1 ; then
      echo "**** FAILED - output didn't match expected output."
      FAIL=1
      return 1
  fi

  echo "PASS"
  return 0
}

# Function to run a single test of the calc25 program.
testCalc25() {
  TESTNO=$1
  ESTATUS=$2

  rm -f output.txt
  
  echo "Test $TESTNO: ./calc25 < input-25-$TESTNO.txt > output.txt"
  ./calc25 < input-25-$TESTNO.txt > output.txt
  STATUS=$?

  # Make sure the program exited with the right exit status.
  if [ $STATUS -ne $ESTATUS ]; then
      echo "**** FAILED - Expected an exit status of $ESTATUS, but got: $STATUS"
      FAIL=1
      return 1
  fi

  # Make sure output matches expected output.
  if ! diff -q expected-25-$TESTNO.txt output.txt >/dev/null 2>&1 ; then
      echo "**** FAILED - output didn't match expected output."
      FAIL=1
      return 1
  fi

  echo "PASS"
  return 0
}

# Try to get a fresh compile of the project.
echo "Running make clean"
make clean

echo "Building calc10 with make"
make calc10
if [ $? -ne 0 ]; then
    echo "**** Make didn't run succesfully when trying to build your calc10 program."
    FAIL=1
fi

# Run tests for calc10
if [ -x calc10 ] ; then
    testCalc10 01 0
    testCalc10 02 0
    testCalc10 03 0
    testCalc10 04 0
    testCalc10 05 0
    testCalc10 06 0
    testCalc10 07 0
    testCalc10 08 0
    testCalc10 09 0
    testCalc10 10 0
    testCalc10 11 0
    testCalc10 12 100
    testCalc10 13 102
    testCalc10 14 101
else
    echo "**** Your calc10 program couldn't be tested since it didn't compile successfully."
    FAIL=1

fi

echo "Building calc25 with make"
make calc25
if [ $? -ne 0 ]; then
    echo "**** Make didn't run succesfully when trying to build your calc25 program."
    FAIL=1
fi

# Run tests for calc25
if [ -x calc25 ] ; then
    testCalc25 01 0
    testCalc25 02 0
    testCalc25 03 0
    testCalc25 04 0
    testCalc25 05 0
    testCalc25 06 0
    testCalc25 07 0
    testCalc25 08 102
    testCalc25 09 100
    testCalc25 10 100
    testCalc25 11 102
else
    echo "**** Your calc25 program couldn't be tested since it didn't compile successfully."
    FAIL=1

fi

if [ $FAIL -ne 0 ]; then
  echo "**** There were failing tests"
  exit 1
else 
  echo "Tests successful"
  exit 0
fi
