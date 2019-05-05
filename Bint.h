//Bint:  -*- C++ -*-  Created: Galindo Sep-2014, Modified: Galindo Sep-2014
 
#ifndef BINT_H
#define BINT_H
 
#include <string>
#include <iostream>
 
class Bint
{
public:
  Bint(): s(0), n() {}
  Bint(const std::string&);
  Bint(long long);
 
  Bint(const Bint& b): s(b.s), n(b.n) {}
  const Bint& operator=(const Bint& b) {s = b.s; n = b.n; return *this;}
 
#if __cplusplus > 201100L
  Bint(Bint&& b): s{b.s}, n{std::move(b.n)} {}
  const Bint& operator=(Bint&& b) {s = b.s; std::swap(n, b.n); return *this;}
#endif
  
  Bint operator+(const Bint& b) const;
  Bint operator-(const Bint& b) const {return *this + (-b);}
  Bint operator-()              const {return Bint(-s, n);}
  Bint operator*(const Bint& b) const;
  Bint operator/(const Bint& b) const;
  Bint operator%(const Bint& b) const;
  
  bool operator==(const Bint& b) const {return s == b.s && n == b.n;}
  bool operator!=(const Bint& b) const {return !(*this == b);}
  bool operator< (const Bint& b) const;
  bool operator<=(const Bint& b) const {return *this == b || *this < b;}
  bool operator> (const Bint& b) const {return !(*this <= b);}
  bool operator>=(const Bint& b) const {return !(*this < b);}
  
  const Bint& operator+=(const Bint& b) {*this = *this + b; return *this;}
  const Bint& operator-=(const Bint& b) {*this = *this - b; return *this;}
  const Bint& operator*=(const Bint& b) {*this = *this * b; return *this;}
  const Bint& operator/=(const Bint& b) {*this = *this / b; return *this;}
  const Bint& operator%=(const Bint& b) {*this = *this % b; return *this;}
  
  Bint& operator++()    {*this += 1; return *this;}
  Bint& operator--()    {*this -= 1; return *this;}
  Bint  operator++(int) {Bint t(*this); *this += 1; return t;}
  Bint  operator--(int) {Bint t(*this); *this -= 1; return t;}
  
  friend std::ostream& operator<<(std::ostream&, const Bint&);
 
  long long  toLong()         const;
  Bint       Bpow(int pot)    const;
  size_t     numberOfDigits() const {return n.size();}
  long long  sumOfDigits()    const;
  Bint       abs()            const {return Bint(s!=0, n);}
  Bint       reverse()        const {std::string t; t.assign(n.rbegin(),n.rend()); return Bint(s, t);}
  bool       isPalindrome()   const {return *this == this->reverse();}
  bool       isEven()         const {return (n[0]-'0')%2==0;}
  bool       isOdd()          const {return (n[0]-'0')%2==1;}
  Bint       factorial()      const;
  bool       isPrime()        const;
  
private:
  int s;         // sign: -1, 0 or 1
  std::string n; // reversed number -aka little-endian format- allows easy carry propagation
 
  Bint(int i, const std::string& str): s(i), n(str) {};
};
 
std::istream& operator>>(std::istream&, Bint&);
 
inline long long  toLong(const Bint& b)         {return b.toLong();}
inline Bint       Bpow(const Bint& b, int p)    {return b.Bpow(p);}
inline size_t     numberOfDigits(const Bint& b) {return b.numberOfDigits();}
inline long long  sumOfDigits(const Bint& b)    {return b.sumOfDigits();}
inline Bint       abs(const Bint& b)            {return b.abs();}
inline Bint       reverse(const Bint& b)        {return b.reverse();}
inline bool       isPalindrome(const Bint& b)   {return b.isPalindrome();}
inline bool       isEven(const Bint& b)         {return b.isEven();}
inline bool       isOdd(const Bint& b)          {return b.isOdd();}
inline Bint       factorial(const Bint& b)      {return b.factorial();}
inline bool       isPrime(const Bint& b)        {return b.isPrime();}
 
#endif /* BINT_H */
