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

# Test the wordle program.
testWordle() {
  TESTNO=$1
  ESTATUS=$2
  SCORES="$3"

  echo "Test $TESTNO"
  rm -f output.txt stderr.txt

  if [ -n "$SCORES" ]; then
      echo "   echo \"$SCORES\" > scores.txt"
      echo "$SCORES" > scores.txt
  else
      echo "   rm -f scores.txt"
      rm -f scores.txt
  fi

  echo "   ./wordle ${args[@]} < input-$TESTNO.txt > output.txt 2> stderr.txt"
  ./wordle ${args[@]} < input-$TESTNO.txt > output.txt 2> stderr.txt
  ASTATUS=$?

  if ! checkStatus "$ESTATUS" "$ASTATUS" ||
     ! checkFile "Stdout output" "expected-output-$TESTNO.txt" "output.txt" ||
     ! checkFileOrEmpty "Stderr output" "expected-stderr-$TESTNO.txt" "stderr.txt" ||
     ! checkFile "scores.txt file" "expected-scores-$TESTNO.txt" "scores.txt"
  then
      FAIL=1
      return 1
  fi

  echo "Test $TESTNO PASS"
  return 0
}

# make a fresh copy of the target programs
make clean
make
if [ $? -ne 0 ]; then
  fail "Make exited unsuccessfully"
fi

# Run test cases
if [ -x wordle ]; then
    args=(list-a.txt 1)
    testWordle 01 0 ""
    
    args=(list-b.txt 1)
    testWordle 02 0 ""
    
    args=(list-c.txt 1)
    testWordle 03 0 ""
    
    args=(list-d.txt 200)
    testWordle 04 0 ""
    
    args=(list-d.txt 400)
    testWordle 05 0 ""
    
    args=(list-d.txt 100)
    testWordle 06 0 ""
    
    args=(list-e.txt 2000)
    testWordle 07 0 ""
    
    args=(list-e.txt 5)
    testWordle 08 0 "0 0 0 2 5 3 1 2 0 0"
    
    args=(list-f.txt 5)
    testWordle 09 0 ""
    
    args=(list-g.txt 11)
    testWordle 10 0 ""
    
    args=(list-e.txt 150)
    testWordle 11 0 "1 1 1 1 1 1 1 1 1 1"
    
    args=(list-e.txt 320)
    testWordle 12 0 "10 10 10 10 10 10 10 10 10 10"

    args=(list-h.txt 1)
    testWordle 13 1 ""
    
    args=(list-i.txt 1)
    testWordle 14 1 ""
    
    args=(list-j.txt 1)
    testWordle 15 1 ""
    
    args=(list-k.txt 1)
    testWordle 16 1 ""
else
    fail "Since your program didn't compile, we couldn't test it"
fi

if [ $FAIL -ne 0 ]; then
  echo "FAILING TESTS!"
  exit 13
else 
  echo "TESTS SUCCESSFUL"
  exit 0
fi
