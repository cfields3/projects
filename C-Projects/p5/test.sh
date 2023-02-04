#!/bin/bash
# This is a shell script to check your program(s) on test cases.
# It usese the same language you normally use to type commands in
# a terminal window.  You can write whole programs in shell.

# Assume we've succeeded until we see otherwise.
FAIL=0

# Print an error message and set the fail flag.
fail() {
    echo "**** $1"
    FAIL=1
}

# Check the exit status.  The actual exit status (ASTATUS) should match
# the expected status (ESTATUS)
checkStatus() {
  ESTATUS="$1"
  ASTATUS="$2"

  # Make sure the program exited with the right exit status.
  if [ "$ASTATUS" -ne "$ESTATUS" ]; then
      fail "FAILED - incorrect program exit status. (expected $ESTATUS,  Got: $ASTATUS)"
      return 1
  fi

  return 0
}

# Check the contents of an a file.  If the expected file (EFILE)
# exists, then the actual file (AFILE) should exist and it should match.
checkFile() {
  NAME="$1"
  EFILE="$2"
  AFILE="$3"

  # Make sure we're really expecting this file.
  if [ ! -f "$EFILE" ]; then
      return 0
  fi

  # Make sure the output matches the expected output.
  echo "   diff $EFILE $AFILE"
  diff -q "$EFILE" "$AFILE" >/dev/null 2>&1
  if [ $? -ne 0 ]; then
      fail "FAILED - $NAME ($AFILE) doesn't match expected ($EFILE)"
      return 1
  fi

  return 0
}

# Same as checkFile, but if the expected file (EFILE) doesn't exist, the
# actual file (AFILE) should not exit either.
checkFileOrMissing() {
  NAME="$1"
  EFILE="$2"
  AFILE="$3"
  
  # if the expected output file doesn't exist, the actual file should't either.
  if [ ! -f "$EFILE" ]; then
      if [ -f "$AFILE" ]; then
	  fail "FAILED - $NAME ($AFILE) should not be created."
	  return 1
      fi
      return 0
  fi

  # Make sure the output matches the expected output.
  echo "   diff $EFILE $AFILE"
  diff -q "$EFILE" "$AFILE" >/dev/null 2>&1
  if [ $? -ne 0 ]; then
      fail "FAILED - $NAME ($AFILE) doesn't match expected ($EFILE)"
      return 1
  fi

  return 0
}

# Same as checkFile, but if the expected file (EFILE) doesn't exist, the
# actual file (AFILE) should be empty.
checkFileOrEmpty() {
  NAME="$1"
  EFILE="$2"
  AFILE="$3"
  
  # if the expected output file doesn't exist, the actual file should be
  # empty.
  if [ ! -f "$EFILE" ]; then
      if [ -s "$AFILE" ]; then
	  fail "FAILED - $NAME ($AFILE) should be empty."
	  return 1
      fi
      return 0
  fi

  # Make sure the output matches the expected output.
  echo "   diff $EFILE $AFILE"
  diff -q "$EFILE" "$AFILE" >/dev/null 2>&1
  if [ $? -ne 0 ]; then
      fail "FAILED - $NAME ($AFILE) doesn't match expected ($EFILE)"
      return 1
  fi

  return 0
}

# The given file, AFILE, should be empty.
checkEmpty() {
  NAME="$1"
  AFILE="$2"
  
  if [ -s "$AFILE" ]; then
      fail "FAILED - $NAME ($AFILE) should be empty."
      return 1
  fi

  return 0
}

# Test the encode program.
testEncode() {
  TESTNO=$1
  ESTATUS=$2

  echo "Encode test $TESTNO"
  rm -f output.txt stdout.txt stderr.txt

  echo "   ./encode ${args[@]} original-$TESTNO.bin output.txt > stdout.txt 2> stderr.txt"
  ./encode ${args[@]} original-$TESTNO.bin output.txt > stdout.txt 2> stderr.txt
  ASTATUS=$?

  if ! checkStatus "$ESTATUS" "$ASTATUS" ||
     ! checkFileOrMissing "Encoded output" "encoded-$TESTNO.txt" "output.txt" ||
     ! checkEmpty "Stdout output" "stdout.txt" ||
     ! checkFileOrEmpty "Stderr output" "expected-stderr-$TESTNO.txt" "stderr.txt"
  then
      FAIL=1
      return 1
  fi

  echo "Encode test $TESTNO PASS"
  return 0
}

# Test the decode program.
testDecode() {
  TESTNO=$1
  ESTATUS=$2

  echo "Decode test $TESTNO"
  rm -f output.bin stdout.txt stderr.txt

  echo "   ./decode ${args[@]} encoded-$TESTNO.txt output.bin > stdout.txt 2> stderr.txt"
  ./decode ${args[@]} encoded-$TESTNO.txt output.bin > stdout.txt 2> stderr.txt
  ASTATUS=$?

  if ! checkStatus "$ESTATUS" "$ASTATUS" ||
     ! checkFileOrMissing "Decoded output" "original-$TESTNO.bin" "output.bin" ||
     ! checkEmpty "Stdout output" "stdout.txt" ||
     ! checkFileOrEmpty "Stderr output" "expected-stderr-$TESTNO.txt" "stderr.txt"
  then
      FAIL=1
      return 1
  fi

  echo "Decode test $TESTNO PASS"
  return 0
}

# make a fresh copy of the target programs
make clean
make
if [ $? -ne 0 ]; then
  fail "Make exited unsuccessfully"
fi

# Test the encode program
if [ -x encode ]; then
    args=()
    testEncode 01 0
    
    args=()
    testEncode 02 0
    
    args=()
    testEncode 03 0
    
    args=()
    testEncode 04 0
    
    args=()
    testEncode 05 0
    
    args=()
    testEncode 06 0
    
    args=()
    testEncode 07 0
    
    args=(-b -p)
    testEncode 08 0
    
    args=(-p -break)
    testEncode 09 1
    
    args=()
    testEncode 10 1
else
    fail "Since your encode program didn't compile, it couldn't be tested."
fi

# Test the decode program
if [ -x decode ]; then
    args=()
    testDecode 01 0
    
    args=()
    testDecode 02 0
    
    args=()
    testDecode 03 0
    
    args=()
    testDecode 04 0
    
    args=()
    testDecode 05 0
    
    args=()
    testDecode 06 0
    
    args=()
    testDecode 07 0
    
    args=()
    testDecode 08 0
    
    args=()
    testDecode 11 1
else
    fail "Since your encode program didn't compile, it couldn't be tested."
fi

if [ $FAIL -ne 0 ]; then
  echo "FAILING TESTS!"
  exit 13
else 
  echo "TESTS SUCCESSFUL"
  exit 0
fi
