///////////////////////////////////////////////////////////////////////////////
// disks.hpp
//
// Definitions for two algorithms that each solve the
// alternating disks problem.
//
// As provided, this header has four functions marked with
// TODO comments.
// You need to write in your own implementation of these
// functions.
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

// State of one disk, either light or dark.
enum disk_color { DISK_LIGHT, DISK_DARK };

// Data structure for the state of one row of disks.
class disk_state {
private:
  std::vector<disk_color> _colors;

public:

  disk_state(size_t light_count)
    : _colors(light_count * 2, DISK_LIGHT) {

      assert(dark_count() > 0);

      for (size_t i = 1; i < _colors.size(); i += 2) {
        _colors[i] = DISK_DARK;
      }
  }

  // Equality operator for unit tests.
  bool operator== (const disk_state& rhs) const {
    return std::equal(_colors.begin(), _colors.end(), rhs._colors.begin());
  }

  size_t total_count() const {
    return _colors.size();
  }

  size_t dark_count() const {
    return total_count() / 2;
  }

  size_t light_count() const {
    return dark_count();
  }

  bool is_index(size_t i) const {
    return (i < total_count());
  }

  disk_color get(size_t index) const {
    assert(is_index(index));
    return _colors[index];
  }

  void swap(size_t left_index) {
    assert(is_index(left_index));
    auto right_index = left_index + 1;
    assert(is_index(right_index));
    std::swap(_colors[left_index], _colors[right_index]);
  }

  std::string to_string() const {
    std::stringstream ss;
    bool first = true;
    for (auto color : _colors) {
      if (!first) {
        ss << " ";
      }

      if (color == DISK_LIGHT) {
        ss << "L";
      } else {
        ss << "D";
      }

      first = false;
    }
    return ss.str();
  }

  // Return true when this disk_state is in alternating format. That means
  // that the first disk at index 0 is light, the second disk at index 1
  // is dark, and so on for the entire row of disks.
  bool is_alternating() const {
    disk_color color = DISK_DARK; //variable for comparing disk color
    for(size_t i = 0; i < _colors.size(); i++){
      if(_colors[i] == color){
        //return false if the node is dark
        return false;
      }
      if(color == DISK_DARK){
        i++;
      }
    }
    return true; //return if node is light
  }

  // Return true when this disk_state is fully sorted, with all light disks
  // on the left (low indices) and all dark disks on the right (high
  // indices).
  bool is_sorted() const {
    disk_color dark = DISK_DARK; //for comparing the colors
    for(size_t i = 0; i < total_count(); i++){
      if(i < total_count() / 2){
        //Makes sure that its comparing the left side
        if(_colors[i] == dark){
          //return false if left side is light
          return false;
        }
        //moves onto the next to check if its dark
      }
    }
   return true; //return true if dark node
  }
};

// Data structure for the output of the alternating disks problem. That
// includes both the final disk_state, as well as a count of the number
// of swaps performed.
class sorted_disks {
private:
  disk_state _after;
  unsigned _swap_count;

public:

  sorted_disks(const disk_state& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  sorted_disks(disk_state&& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  const disk_state& after() const {
    return _after;
  }

  unsigned swap_count() const {
    return _swap_count;
  }
};

// Algorithm that sorts disks using the left-to-right algorithm.
sorted_disks sort_left_to_right(const disk_state& before) {
  int swapCount = 0; //for counting the swaps
  disk_state temporary = before; //temp hold

  for(size_t i = 0; i < temporary.dark_count(); i++){
    for(size_t j = i; j < temporary.total_count() - 1; j++){
      //compare if it's light and dark
      if(temporary.get(j) > temporary.get(j+1)){
        temporary.swap(j); //swap
        swapCount++;
      }
    }
  }
  // check that the input is in alternating format
  assert(before.is_alternating());
  //return the state and swap count
  return sorted_disks(disk_state(temporary), swapCount);
}

// Algorithm that sorts disks using the lawnmower algorithm.
sorted_disks sort_lawnmower(const disk_state& before) {
  int swapCount = 0; //counter for swaps
  disk_state temporary = before; //temp hold
  bool increment;

  for(size_t i = 0; i < temporary.dark_count(); i++){
    increment = (i%2 == 0)? true:false; //if its an even, return true, else false
    //let j = i when even and increment, else decrement
    for(size_t j = increment? i: temporary.total_count() - 2; j < temporary.total_count() - 1; increment? j++ : j--){
      if(temporary.get(j) > temporary.get(j+1)){
        temporary.swap(j); //swap nodes
        swapCount++;
      }
    }
  }
  // check that the input is in alternating format
  assert(before.is_alternating());

  //return the state and swap count
  return sorted_disks(disk_state(temporary), swapCount);
}
