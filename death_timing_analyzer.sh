#!/bin/bash

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Configuration
PHILO="./philosophers"
RESULTS_DIR="death_timing_results"
mkdir -p $RESULTS_DIR

# Function to parse timestamp from output line
parse_timestamp() {
    echo "$1" | awk '{print $1}' | tr -d '[]'
}

# Function to analyze death timing
analyze_death_timing() {
    local num_philo=$1
    local time_to_die=$2
    local time_to_eat=$3
    local time_to_sleep=$4
    local test_name=$5
    local log_file="$RESULTS_DIR/${test_name// /_}.log"
    
    echo -e "\n${CYAN}=== Testing: $test_name ===${NC}"
    echo -e "${YELLOW}Command:${NC} $PHILO $num_philo $time_to_die $time_to_eat $time_to_sleep"
    echo -e "${YELLOW}Expected death time:${NC} ~${time_to_die}ms\n"
    
    # Run the program and capture output
    output=$(timeout 10s $PHILO $num_philo $time_to_die $time_to_eat $time_to_sleep 2>&1)
    echo "$output" > "$log_file"
    
    # Find the death line
    death_line=$(echo "$output" | grep "died" | head -1)
    
    if [ -z "$death_line" ]; then
        echo -e "${RED}[ERROR]${NC} No philosopher died in this test"
        return 1
    fi
    
    # Extract philosopher ID and death timestamp
    philo_id=$(echo "$death_line" | awk '{print $2}')
    death_time=$(parse_timestamp "$death_line")
    
    echo -e "${PURPLE}Death detected:${NC}"
    echo -e "  Philosopher ID: $philo_id"
    echo -e "  Death time: ${death_time}ms"
    echo -e "  Death line: $death_line"
    
    # Find last eating event for this philosopher
    last_eat_line=$(echo "$output" | grep "^.*$philo_id is eating" | tail -1)
    
    if [ -n "$last_eat_line" ]; then
        last_eat_time=$(parse_timestamp "$last_eat_line")
        echo -e "\n${PURPLE}Last meal:${NC}"
        echo -e "  Last eat time: ${last_eat_time}ms"
        echo -e "  Last eat line: $last_eat_line"
        
        # Calculate time difference
        time_diff=$((death_time - last_eat_time))
        echo -e "\n${PURPLE}Time analysis:${NC}"
        echo -e "  Time between last meal and death: ${time_diff}ms"
        echo -e "  Expected time_to_die: ${time_to_die}ms"
        echo -e "  Difference: $((time_diff - time_to_die))ms"
        
        # Tolerance check (±10ms is acceptable)
        tolerance=10
        if [ $time_diff -ge $((time_to_die - tolerance)) ] && [ $time_diff -le $((time_to_die + tolerance)) ]; then
            echo -e "\n${GREEN}[PASS]${NC} Death timing is within acceptable range (${time_to_die}±${tolerance}ms)"
        else
            echo -e "\n${RED}[FAIL]${NC} Death timing is outside acceptable range"
            echo -e "  Expected: ${time_to_die}±${tolerance}ms"
            echo -e "  Actual: ${time_diff}ms"
        fi
    else
        # Check if philosopher started eating at all
        first_eat=$(echo "$output" | grep "^.*$philo_id is eating" | head -1)
        if [ -z "$first_eat" ]; then
            echo -e "\n${YELLOW}[INFO]${NC} Philosopher $philo_id never ate (expected for 1 philosopher test)"
            # For single philosopher, death should occur at time_to_die
            if [ $death_time -ge $((time_to_die - 10)) ] && [ $death_time -le $((time_to_die + 10)) ]; then
                echo -e "${GREEN}[PASS]${NC} Death timing correct for philosopher who never ate"
            else
                echo -e "${RED}[FAIL]${NC} Death timing incorrect (expected ~${time_to_die}ms, got ${death_time}ms)"
            fi
        fi
    fi
    
    # Additional analysis: Check all eating patterns
    echo -e "\n${BLUE}=== Full Eating Timeline for Philosopher $philo_id ===${NC}"
    eating_events=$(echo "$output" | grep "^.*$philo_id is eating" | nl -nln -w1)
    if [ -n "$eating_events" ]; then
        echo "$eating_events"
        
        # Calculate intervals between meals
        echo -e "\n${BLUE}=== Meal Intervals ===${NC}"
        prev_time=0
        while IFS= read -r line; do
            curr_time=$(parse_timestamp "$(echo "$line" | cut -f2-)")
            if [ $prev_time -ne 0 ]; then
                interval=$((curr_time - prev_time))
                echo "Interval: ${interval}ms (between meals)"
            fi
            prev_time=$curr_time
        done <<< "$eating_events"
    else
        echo "No eating events recorded"
    fi
    
    echo -e "\n${BLUE}=== Test Summary ===${NC}"
    echo "Configuration: $num_philo philosophers, ${time_to_die}ms to die, ${time_to_eat}ms to eat, ${time_to_sleep}ms to sleep"
    echo "Log saved to: $log_file"
    echo -e "${CYAN}$("="*50)${NC}\n"
}

# Function to run batch tests
run_death_tests() {
    echo -e "${PURPLE}Starting Death Timing Analysis${NC}"
    echo -e "Testing binary: $PHILO"
    echo -e "Results directory: $RESULTS_DIR\n"
    
    # Check if philo binary exists
    if [ ! -f "$PHILO" ]; then
        echo -e "${RED}Error: $PHILO not found!${NC}"
        exit 1
    fi
    
    # Test cases with different configurations
    # Format: num_philo time_to_die time_to_eat time_to_sleep "test_name"
    declare -a test_cases=(
        "1 200 100 100 Single_philosopher_200ms"
        "1 400 200 200 Single_philosopher_400ms"
        "2 100 200 200 Two_philosophers_quick_death"
        "2 200 100 100 Two_philosophers_normal_death"
        "3 100 150 150 Three_philosophers_tight_timing"
        "3 200 100 100 Three_philosophers_normal_timing"
        "4 310 200 100 Four_philosophers_expected_death"
        "4 410 200 200 Four_philosophers_longer_life"
        "5 100 100 100 Five_philosophers_quick_death"
        "5 200 100 100 Five_philosophers_normal_death"
        "5 800 200 200 Five_philosophers_long_life"
    )
    
    # Run each test case
    for test_case in "${test_cases[@]}"; do
        IFS=' ' read -r num_philo time_to_die time_to_eat time_to_sleep test_name <<< "$test_case"
        analyze_death_timing $num_philo $time_to_die $time_to_eat $time_to_sleep "$test_name"
        sleep 1  # Small delay between tests
    done
    
    # Generate summary report
    echo -e "\n${PURPLE}=== FINAL SUMMARY REPORT ===${NC}"
    echo "All test logs saved in: $RESULTS_DIR/"
    echo "You can examine individual logs for detailed timing information"
    
    # Create a consolidated report
    summary_file="$RESULTS_DIR/summary_report.txt"
    echo "Death Timing Analysis Summary" > $summary_file
    echo "Generated on: $(date)" >> $summary_file
    echo "" >> $summary_file
    
    for log_file in $RESULTS_DIR/*.log; do
        if [ -f "$log_file" ]; then
            test_name=$(basename "$log_file" .log)
            echo "=== $test_name ===" >> $summary_file
            
            death_line=$(grep "died" "$log_file" | head -1)
            if [ -n "$death_line" ]; then
                echo "Death: $death_line" >> $summary_file
                philo_id=$(echo "$death_line" | awk '{print $2}')
                last_eat=$(grep "^.*$philo_id is eating" "$log_file" | tail -1)
                if [ -n "$last_eat" ]; then
                    echo "Last eat: $last_eat" >> $summary_file
                fi
            else
                echo "No death recorded" >> $summary_file
            fi
            echo "" >> $summary_file
        fi
    done
    
    echo -e "${GREEN}Summary report created:${NC} $summary_file"
}

# Function for interactive single test
interactive_test() {
    echo -e "${CYAN}Interactive Death Timing Test${NC}"
    echo "Enter test parameters:"
    
    read -p "Number of philosophers: " num_philo
    read -p "Time to die (ms): " time_to_die
    read -p "Time to eat (ms): " time_to_eat
    read -p "Time to sleep (ms): " time_to_sleep
    read -p "Test name: " test_name
    
    analyze_death_timing $num_philo $time_to_die $time_to_eat $time_to_sleep "$test_name"
}

# Main menu
main() {
    echo -e "${PURPLE}Philosophers Death Timing Analyzer${NC}"
    echo "1. Run all predefined test cases"
    echo "2. Run interactive single test"
    echo "3. Exit"
    
    read -p "Select option: " option
    
    case $option in
        1)
            run_death_tests
            ;;
        2)
            interactive_test
            ;;
        3)
            echo "Exiting..."
            exit 0
            ;;
        *)
            echo "Invalid option"
            exit 1
            ;;
    esac
}

# Check if running with arguments or interactive
if [ $# -eq 0 ]; then
    main
else
    # Allow direct test execution
    if [ $# -eq 5 ]; then
        analyze_death_timing $1 $2 $3 $4 "$5"
    else
        echo "Usage: $0 [num_philo time_to_die time_to_eat time_to_sleep \"test_name\"]"
        echo "Or run without arguments for interactive mode"
        exit 1
    fi
fi
