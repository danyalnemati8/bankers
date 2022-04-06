//
//  customer.h
//  bankers
//
//  Created by William McCarthy on 21 Mar 2022.
//

#ifndef __customer_h__
#define __customer_h__

#include "bank.h"
#include "utils.h"

class Bank;


class Customer {
public:
//  Customer() = default;
  Customer(int index, const std::vector_<int>& allocate, const std::vector_<int>& maximum, Bank* bank_=nullptr)
  : idx(index), alloc(allocate), max_(maximum), need(max_ - alloc), bank(bank_) { }
  
  int get_id() const { return idx; }
  pthread_t* get_threadid() { return &thread_id; }
  
  Bank* get_bank() const { return bank; }
  
  bool needs_met() const { return alloc == max_; }
  bool too_much(const vector_<int>& req) const { 
    int n = req.size();
    for (int i = 0; i < n; ++i) { 
      if (req[i] > need[i]) { return true; }
    }
    return false; 
  }
  
  int get_idx() const { return idx; }
  const vector_<int>& get_alloc() const { return alloc; }
  const vector_<int>& get_max()   const { return max_; }
  const vector_<int>& get_need()  const { return need; }

  void alloc_req(  const vector_<int>& req) { alloc += req;  need -= req; }
  void dealloc_req(const vector_<int>& req) { alloc -= req;  need += req; }
  
  void release_all_resources() {
    vector_<int> zero(alloc.size(), 0);
    alloc = max_ = need = zero;
    Utils::print_locked("Customer p#%d has released all resources and is shutting down\n", idx);
  }

  void show() const {
    Utils::print_locked("P#%d    %s    %s    %s\n", idx, (alloc.as_string().c_str()), (max_.as_string().c_str()), 
                (need.as_string().c_str()));
  }

  friend std::ostream& operator<<(std::ostream& os, const Customer& c) {
    c.show();
    return os;
  }

  vector_<int> create_req() {    // TODO:  must be updated to make more reasonable requests
    vector_<int> req;
    for (size_t i = 0; i < alloc.size(); ++i) {
      if (need[i] == 0) { req.push_back(0);  continue; }
      int val = (int)((rand() % 3) + 1);
      req.push_back(val);
    }
    return req;
  }

private:
  int idx;
  vector_<int> alloc;
  std::vector_<int> max_;
  std::vector_<int> need;
  pthread_t thread_id;
  
  Bank* bank;
};

#endif /* customer.h */
//Output
/*
buf is: 7, 7, 8
buf is: 3, 2, 1,    3, 3, 3
buf is: 1, 2, 1,    2, 2, 2
buf is: 1, 1, 1,    1, 3, 1
buf is: 0, 0, 1,    1, 1, 1
buf is:

Banker's algorithm simulation beginning...
--------------------------------------------

+-----------------------------------------
|     BANK   avail: [7  7  8]
+-----------------------------------------
| P#  0     3  2  1    3  3  3    0  1  2 
| P#  1     1  2  1    2  2  2    1  0  1
| P#  2     1  1  1    1  3  1    0  2  0
| P#  3     0  0  1    1  1  1    1  1  0
+-----------------------------------------
<<< Customer thread p#0 started... >>>
<<< Customer thread p#1 started... >>>
<<< Customer thread p#2 started... >>>
<<< Customer thread p#3 started... >>>
P0 requesting : 0  2  2   --- Request DENIED
P1 requesting : 3  0  3   --- Request DENIED
P2 requesting : 0  2  0   .......... request APPROVED!

+-----------------------------------------
|     BANK   avail: [7  5  8]
+-----------------------------------------
| P#  0     3  2  1    3  3  3    0  1  2 
| P#  1     1  2  1    2  2  2    1  0  1
| P#  2     1  3  1    1  3  1    0  0  0
| P#  3     0  0  1    1  1  1    1  1  0
+-----------------------------------------

+-----------------------------------------
|     BANK   avail: [10 10 10]
+-----------------------------------------
| P#  0     0  0  0    0  0  0    0  0  0 
| P#  1     2  2  2    2  2  2    0  0  0
| P#  2     0  0  0    0  0  0    0  0  0
| P#  3     0  0  0    0  0  0    0  0  0
+-----------------------------------------
Customer p#1 has released all resources and is shutting down

+-----------------------------------------
|     BANK   avail: [12 12 12]
+-----------------------------------------
| P#  0     0  0  0    0  0  0    0  0  0 
| P#  1     0  0  0    0  0  0    0  0  0
| P#  2     0  0  0    0  0  0    0  0  0
| P#  3     0  0  0    0  0  0    0  0  0
+-----------------------------------------
>>>>>>>>>>>>>>> Customer thread p#1 shutting down... >>>>>>>>>>>>>>>

+-----------------------------------------
|     BANK   avail: [12 12 12]
+-----------------------------------------
| P#  0     0  0  0    0  0  0    0  0  0 
| P#  1     0  0  0    0  0  0    0  0  0
| P#  2     0  0  0    0  0  0    0  0  0
| P#  3     0  0  0    0  0  0    0  0  0
+-----------------------------------------

Banker's algorithm simulation completed...

    ...done


*/