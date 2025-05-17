#!/bin/bash

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color
BLUE='\033[0;34m'

# Configuration
PHILO="./philosophers"
TIMEOUT_CMD="timeout"
VALGRIND_CMD="valgrind"

# Test counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Create results directory
RESULTS_DIR="test_results"
mkdir -p $RESULTS_DIR

# Function to print test header
print_header() {
    echo -e "\n${BLUE}=== $1 ===${NC}"
}

# Function to print test result
print_result() {
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    if [ $1 -eq 0 ]; then
        echo -e "${GREEN}[PASS]${NC} $2"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}[FAIL]${NC} $2"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
}

# Function to check if philosopher should die
test_death() {
    local num_philo=$1
    local time_to_die=$2
    local time_to_eat=$3
    local time_to_sleep=$4
    local test_name=$5
    local expected_death=$6
    
    echo -e "\n${YELLOW}Testing:${NC} $test_name"
    echo "Command: $PHILO $num_philo $time_to_die $time_to_eat $time_to_sleep"
    
    output=$($TIMEOUT_CMD 10s $PHILO $num_philo $time_to_die $time_to_eat $time_to_sleep 2>&1)
    exit_code=$?
    
    # Save output for debugging
    echo "$output" > "$RESULTS_DIR/${test_name// /_}.log"
    
    died=$(echo "$output" | grep -c "died")
    
    if [ $expected_death -eq 1 ]; then
        if [ $died -gt 0 ]; then
            print_result 0 "$test_name - Philosopher died as expected"
        else
            print_result 1 "$test_name - Philosopher should have died but didn't"
        fi
    else
        if [ $died -eq 0 ]; then
            print_result 0 "$test_name - No philosopher died (correct)"
        else
            print_result 1 "$test_name - Philosopher died unexpectedly"
        fi
    fi
}

# Function to test with eat count
test_eat_count() {
    local num_philo=$1
    local time_to_die=$2
    local time_to_eat=$3
    local time_to_sleep=$4
    local must_eat=$5
    local test_name=$6
    
    echo -e "\n${YELLOW}Testing:${NC} $test_name"
    echo "Command: $PHILO $num_philo $time_to_die $time_to_eat $time_to_sleep $must_eat"
    
    start_time=$(date +%s)
    output=$($TIMEOUT_CMD 10s $PHILO $num_philo $time_to_die $time_to_eat $time_to_sleep $must_eat 2>&1)
    exit_code=$?
    end_time=$(date +%s)
    duration=$((end_time - start_time))
    
    # Save output for debugging
    echo "$output" > "$RESULTS_DIR/${test_name// /_}.log"
    
    died=$(echo "$output" | grep -c "died")
    
    if [ $died -eq 0 ] && [ $exit_code -eq 0 ]; then
        print_result 0 "$test_name - Program finished correctly (duration: ${duration}s)"
    else
        print_result 1 "$test_name - Program failed or philosopher died"
    fi
}

# Function to test invalid arguments
test_invalid_args() {
    local args="$1"
    local test_name="$2"
    
    echo -e "\n${YELLOW}Testing:${NC} $test_name"
    echo "Command: $PHILO $args"
    
    output=$($PHILO $args 2>&1)
    exit_code=$?
    
    if [ $exit_code -ne 0 ]; then
        print_result 0 "$test_name - Correctly rejected invalid arguments"
    else
        print_result 1 "$test_name - Failed to reject invalid arguments"
    fi
}

# Function to test with valgrind
test_valgrind() {
    local args="$1"
    local test_name="$2"
    
    if ! command -v $VALGRIND_CMD &> /dev/null; then
        echo -e "${YELLOW}[SKIP]${NC} $test_name - Valgrind not installed"
        return
    fi
    
    echo -e "\n${YELLOW}Testing:${NC} $test_name"
    echo "Command: $VALGRIND_CMD --leak-check=full --show-leak-kinds=all $PHILO $args"
    
    output=$($VALGRIND_CMD --leak-check=full --show-leak-kinds=all --error-exitcode=1 \
           $TIMEOUT_CMD 5s $PHILO $args 2>&1)
    valgrind_exit=$?
    
    # Save output for debugging
    echo "$output" > "$RESULTS_DIR/${test_name// /_}_valgrind.log"
    
    leaks=$(echo "$output" | grep -c "definitely lost\|indirectly lost")
    errors=$(echo "$output" | grep -c "ERROR SUMMARY: [1-9]")
    
    if [ $leaks -eq 0 ] && [ $errors -eq 0 ]; then
        print_result 0 "$test_name - No memory leaks or errors"
    else
        print_result 1 "$test_name - Memory leaks or errors detected"
    fi
}

# Function to test race conditions with helgrind
test_helgrind() {
    local args="$1"
    local test_name="$2"
    
    if ! command -v $VALGRIND_CMD &> /dev/null; then
        echo -e "${YELLOW}[SKIP]${NC} $test_name - Valgrind not installed"
        return
    fi
    
    echo -e "\n${YELLOW}Testing:${NC} $test_name"
    echo "Command: $VALGRIND_CMD --tool=helgrind $PHILO $args"
    
    output=$($VALGRIND_CMD --tool=helgrind --error-exitcode=1 \
           $TIMEOUT_CMD 5s $PHILO $args 2>&1)
    helgrind_exit=$?

    # Save output for debugging
    echo "$output" > "$RESULTS_DIR/${test_name// /_}_helgrind.log"
    
    races=$(echo "$output" | grep -c "data race")
    
    if [ $races -eq 0 ] && [ $helgrind_exit -eq 0 ] || [ $helgrind_exit -eq 124 ]; then
        print_result 0 "$test_name - No race conditions detected"
    else
        print_result 1 "$test_name - Race conditions detected"
    fi
}

# Main test execution
main() {
    echo -e "${BLUE}Starting Philosophers Test Suite${NC}"
    echo "Testing binary: $PHILO"
    echo "Results will be saved in: $RESULTS_DIR"
    
    # Check if philo binary exists
    if [ ! -f "$PHILO" ]; then
        echo -e "${RED}Error: $PHILO not found!${NC}"
        exit 1
    fi
    
   # Test 1: Invalid arguments
    print_header "Invalid Arguments Tests"
    test_invalid_args "" "No arguments"
    test_invalid_args "1" "Too few arguments"
    test_invalid_args "1 2" "Too few arguments"
    test_invalid_args "1 2 3" "Too few arguments"
    test_invalid_args "0 200 100 100" "Zero philosophers"
    test_invalid_args "-1 200 100 100" "Negative philosophers"
    test_invalid_args "1 -200 100 100" "Negative time_to_die"
    test_invalid_args "1 200 -100 100" "Negative time_to_eat"
    test_invalid_args "1 200 100 -100" "Negative time_to_sleep"
    test_invalid_args "1 200 100 100 -1" "Negative must_eat"
    test_invalid_args "abc 200 100 100" "Non-numeric argument"
    test_invalid_args "201 200 100 100" "Too many philosophers"
    
    # Test 2: Death tests
    print_header "Death Tests"
    test_death 1 200 100 100 "One philosopher should die" 1
    test_death 2 100 200 200 "Two philosophers should die quickly" 1
    test_death 3 100 150 150 "Three philosophers - one should die" 1
    test_death 4 310 200 100 "Four philosophers - one should die" 1
    test_death 5 100 100 100 "Five philosophers - one should die" 1
    
    # Test 3: Survival tests
    print_header "Survival Tests (medium times)"
    test_death 2 203 100 100 "Two philosophers should survive" 0
    test_death 3 303 100 100 "Three philosophers should survive" 0
    test_death 4 403 200 200 "Four philosophers should survive" 0
    test_death 5 603 200 200 "Five philosophers should survive" 0
    print_header "Survival Tests (litel times)"
    test_death 2 23 10 10 "Two philosophers should survive" 0
    test_death 3 33 10 10 "Three philosophers should survive" 0
    test_death 4 43 20 20 "Four philosophers should survive" 0
    test_death 5 63 20 20 "Five philosophers should survive" 0
        print_header "Survival Tests (many people)"
    test_death 100 26 10 10 "all should survive" 0
    test_death 101 36 10 10 "all should survive" 0
    test_death 100 46 20 20 "Four philosophers should survive" 0
    test_death 100 203 100 100 "all should survive" 0
    test_death 101 303 100 100 "all should survive" 0
    test_death 100 403 200 150 "Four philosophers should survive" 0
        print_header "Survival Tests (async)"
    test_death 5 63 20 7 "Five philosophers should survive" 0
    test_death 2 203 100 35 "Two philosophers should survive" 0
    test_death 11 303 100 200 "Three philosophers should survive" 0
    test_death 4 303 100 200 "Four philosophers should survive" 0
    test_death 5 603 200 200 "Five philosophers should survive" 0
    # Test 4: Eat count tests
    print_header "Eat Count Tests"
    test_eat_count 3 400 100 100 3 "3 philosophers eat 3 times"
    test_eat_count 4 410 200 200 5 "4 philosophers eat 5 times"
    test_eat_count 5 800 200 200 7 "5 philosophers eat 7 times"
    test_eat_count 2 400 100 100 10 "2 philosophers eat 10 times"
    
    # Test 5: Edge cases
    print_header "Edge Cases"
    test_death 1 400 200 200 "Single philosopher (should die)" 1
    test_eat_count 3 400 100 100 1 "Eat only once"
    test_eat_count 4 410 200 200 100 "Eat many times"
    
    # Test 6: Performance tests
    print_header "Performance Tests"
    test_eat_count 199 800 200 200 5 "Many philosophers (199)"
    test_eat_count 200 800 200 200 5 "Maximum philosophers (200)"
    
    # Test 7: Memory leak tests
    print_header "Memory Leak Tests"
    test_valgrind "4 410 200 200 5" "Memory leaks with eat count"
    test_valgrind "4 200 100 100" "Memory leaks with death"

    # Test 8: Race condition tests
    print_header "______RACE CONDITION TESTS_____"
    test_helgrind "4 410 200 200 5" "Race conditions check"

    test_helgrind "4 410 200 200 5" "Memory leaks with eat count"
    test_helgrind "4 200 100 100" "Memory leaks with death"
    # Memory race tests for all survival tests (medium times)
    test_helgrind "2 203 100 100" "Memory leaks - 2 philosophers should survive (medium times)"
    test_helgrind "3 303 100 100" "Memory leaks - 3 philosophers should survive (medium times)"
    test_helgrind "4 403 200 200" "Memory leaks - 4 philosophers should survive (medium times)"
    test_helgrind "5 603 200 200" "Memory leaks - 5 philosophers should survive (medium times)"

    # Memory race tests for all survival tests (litel times)
    test_helgrind "2 23 10 10" "Memory leaks - 2 philosophers should survive (litel times)"
    test_helgrind "3 33 10 10" "Memory leaks - 3 philosophers should survive (litel times)"
    test_helgrind "4 43 20 20" "Memory leaks - 4 philosophers should survive (litel times)"
    test_helgrind "5 63 20 20" "Memory leaks - 5 philosophers should survive (litel times)"

    # Memory race tests for all survival tests (many people)
    test_helgrind "100 26 10 10" "Memory leaks - 100 philosophers should survive (many people)"
    test_helgrind "101 36 10 10" "Memory leaks - 101 philosophers should survive (many people)"
    test_helgrind "100 46 20 20" "Memory leaks - 100 philosophers should survive (many people)"
    test_helgrind "100 203 100 100" "Memory leaks - 100 philosophers should survive (many people)"
    test_helgrind "101 303 100 100" "Memory leaks - 101 philosophers should survive (many people)"
    test_helgrind "100 403 200 150" "Memory leaks - 100 philosophers should survive (many people)"

    # Memory race tests for all survival tests (async)
    test_helgrind "5 63 20 7" "Memory leaks - 5 philosophers should survive (async)"
    test_helgrind "2 203 100 35" "Memory leaks - 2 philosophers should survive (async)"
    test_helgrind "11 303 100 200" "Memory leaks - 11 philosophers should survive (async)"
    test_helgrind "4 303 100 200" "Memory leaks - 4 philosophers should survive (async)"
    test_helgrind "5 603 200 200" "Memory leaks - 5 philosophers should survive (async)"
    test_helgrind "11 206 3 200" "Memory leaks - 11 philosophers should survive (async)"

    # Existing race leak tests for eat count, edge, and performance
    test_helgrind "3 400 100 100 3" "Memory leaks - 3 philosophers eat 3 times"
    test_helgrind "4 410 200 200 5" "Memory leaks - 4 philosophers eat 5 times"
    test_helgrind "5 800 200 200 7" "Memory leaks - 5 philosophers eat 7 times"
    test_helgrind "2 400 100 100 10" "Memory leaks - 2 philosophers eat 10 times"
    test_helgrind "3 400 100 100 1" "Memory leaks - Eat only once"
    test_helgrind "4 410 200 200 100" "Memory leaks - Eat many times"
    test_helgrind "199 800 200 200 5" "Memory leaks - Many philosophers (199)"
    test_helgrind "200 800 200 200 5" "Memory leaks - Maximum philosophers (200)"

    # Summary
    echo -e "\n${BLUE}==== TEST SUMMARY ====${NC}"
    echo -e "Total tests: $TOTAL_TESTS"
    echo -e "${GREEN}Passed: $PASSED_TESTS${NC}"
    echo -e "${RED}Failed: $FAILED_TESTS${NC}"
    
    if [ $FAILED_TESTS -eq 0 ]; then
        echo -e "\n${GREEN}All tests passed! 🎉${NC}"
        exit 0
    else
        echo -e "\n${RED}Some tests failed. Check $RESULTS_DIR for detailed logs.${NC}"
        exit 1
    fi
}

# Run main function
main "$@"