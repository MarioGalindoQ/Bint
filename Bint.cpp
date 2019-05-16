//Bint:  -*- C++ -*-  Created: Galindo Sep-2014, Modified: Galindo Sep-2014
 
#include <cstdlib> // exit
#include <limits>  // numeric_limits
#include <vector>
#include "Bint.h"
 
using namespace std;
 
static bool lt(const string& n1, const string& n2)
{
  if(n1.size() < n2.size()) return true;
  if(n1.size() > n2.size()) return false;
  if(n1.size() == 0) return false;
 
  for(int i = n1.size()-1; i>=0; --i)
    {
      if(n1[i] < n2[i]) return true;
      if(n1[i] > n2[i]) return false;
    }
 
  return false;
}
 
static string longToString(long long a)
{
  string n;
 
  if(a<=9999999999) n.reserve(10);
  else              n.reserve(20);
 
  while(a) {n += a%10 + '0'; a /= 10;}
 
  return n;
}
 
static const string maxlong = longToString(std::numeric_limits<long long>::max()-1);
static const int maxlongsize = maxlong.size();
 
static long long stringToLong(const string& n)
{
  if(lt(maxlong, n))
    {cerr << "Can't be expressed as long long: " << n << endl; exit(1);}
 
  long long res=0, p=1;
  for(size_t i=0; i<n.size(); i++) {res += (n[i]-'0')*p; p*=10;}
 
  return res;
}
 
static string add(const string& n1, const string& n2)
{
  const string& min = n1.size()<n2.size() ? n1 : n2;
  const string& max = n1.size()<n2.size() ? n2 : n1;
 
  int minsize = min.size();
  int maxsize = max.size();
 
  string res;
  res.reserve(maxsize+1);
 
  int c=0;
  for(int j=0; j<minsize; ++j)
    {
      int s = c + max[j] + min[j] - 2*'0';
      c = s/10;
      res += '0' + s%10;
    }
  for(int j=minsize; j<maxsize; ++j)
    {
      int s = c + max[j] - '0';
      c = s/10;
      res += '0' + s%10;
    }
  if(c) res += '0' + c;
 
  return res;
}
 
static string sub(const string& n1, const string& n2, bool ltn1n2)
{
  const string& min = ltn1n2 ? n1 : n2;
  const string& max = ltn1n2 ? n2 : n1;
 
  int minsize = min.size();
  int maxsize = max.size();
 
  string res;
  res.reserve(maxsize);
 
  int s, c=0;
  for(int j=0; j<minsize; ++j)
    {
      s = c + max[j] - min[j];
      if(s<0) {c=-1; s+=10;}
      else    c=0;
      res += '0' + s;
    }
  for(int j=minsize; j<maxsize; ++j)
    {
      s = c + max[j] - '0';
      if(s<0) {c=-1; s+=10;}
      else    c=0;
      res += '0' + s;
    }
 
  int ressize = res.size();
  int i=0;
  while(res[ressize-1-i]=='0') ++i;
  if(i>0) res.erase(ressize-i,i);
 
  return res;
}
 
static string mul(const string& n1, char n2, int p)
{
  int n1size = n1.size();
 
  string res;
  res.reserve(n1size+p+1);
 
  while(p--) res += '0';
 
  int f = n2-'0';
  int c=0;
  for(int j=0; j<n1size; ++j)
    {
      int s = c + f*(n1[j]-'0');
      c = s/10;
      res += '0' + s%10;
    }
  if(c) res += '0'+c;
 
  return res;
}
 
Bint::Bint(const string& str) : s(1), n()
{
  string t(str);
 
  if(t[0] == '-') {s = -1; t.erase(0,1);}
 
  int i=0;
  while(t[i]=='0') ++i;
  if(i>0) t.erase(0,i);
 
  if(t.size()==0) {s=0; return;}
 
  n.assign(t.rbegin(), t.rend()); // Reverse
}
 
Bint::Bint(long long a): s(0), n()
{
  if(a==0) return;
 
  if(a<0) {s=-1; a*=-1;}
  else    s=1;
 
  n = longToString(a);
}
 
Bint Bint::operator+(const Bint& b) const
{
  if(s == 0)   return b;
  if(b.s == 0) return *this;
 
  // long long is slower due to convertions.
  
  Bint sum;
  if(s == b.s)
    {
      sum.n = add(n, b.n);
      sum.s = s;
    }
  else
    {
      if(n == b.n) return 0;
      
      bool ltnb = lt(n, b.n);
      sum.n = sub(n, b.n, ltnb);
      sum.s = ltnb ? b.s : s;
    }
  
  return sum;
}
 
Bint Bint::operator*(const Bint& b) const
{
  if(s==0 || b.s==0) return 0;
 
  if(b.n=="1" || n=="1")
    {
      if(b==1)      return  *this;
      if(b==-1)     return -*this;
      if(*this==1)  return  b;
      if(*this==-1) return -b;
    }
 
  int maxLen = n.size()>b.n.size() ? n.size() : b.n.size();
  if(maxLen < maxlongsize/2) // Use fast long long
    {
      return s*stringToLong(n) * b.s*stringToLong(b.n);
    }
  else // Use slow Bint
    {
      Bint res;
      if(s==b.s) res.s = 1;
      else       res.s =-1;
 
      res.n = mul(n, b.n[0], 0);
 
      int bnsize = b.n.size();
      for(int i=1; i<bnsize; ++i) res.n = add(res.n, mul(n, b.n[i], i));
      
      return res;
    }
}
 
Bint Bint::operator/(const Bint& b) const
{
  if(b.s==0) {cerr << "Division by zero error\n"; exit(1);}
  if(s==0 || lt(n, b.n)) return 0;
  if(b.n=="1")
    {
      if(b.s== 1) return  *this;
      if(b.s==-1) return -*this;
    }
 
  if(lt(this->n, maxlong) && lt(b.n, maxlong)) // Use fast long long
    {
      return s*stringToLong(n)/(b.s*stringToLong(b.n));
    }
  else // Use slow Bint
    {
      Bint bb = b.abs();
      vector<Bint> tab(11);
      for(int i = 0; i < 11; ++i) tab[i] = Bint(i)*bb;
 
      Bint ratio(0), resi(1, n.substr(n.size()-b.n.size(),b.n.size()));
      for(int i = n.size()-b.n.size(); i >= 0; --i)
        {
          int count = 0;
          while(tab[count] < resi) ++count;
          if(tab[count] != resi)   --count;
          
          ratio = ratio * Bint(10) + Bint(count);
          if(count) resi = resi - tab[count];
          
          if(i>0) resi = resi * Bint(10) + Bint(n[i-1]-'0');
        }
      
      if(s!=b.s) ratio.s = -1;
      
      return ratio;
    }
}
 
Bint Bint::operator%(const Bint& b) const
{
  if(b.s==0) {cerr << "Division by zero error\n"; exit(1);}
  if(s==0 || lt(n, b.n)) return *this;
  if(b.n=="1")           return 0;
 
  if(lt(this->n, maxlong) && lt(b.n, maxlong)) // Use fast long long
    {
      return s*stringToLong(n)%stringToLong(b.n);
    }
  else // Use slow Bint
    {
      Bint bb = b.abs();
      vector<Bint> tab(11);
      for(int i = 0; i < 11; ++i) tab[i] = Bint(i)*bb;
      
      Bint ratio(0), resi(1, n.substr(n.size()-b.n.size(),b.n.size()));
      for(int i = n.size()-b.n.size(); i >= 0; --i)
        {
          int count = 0;
          while(tab[count] < resi) ++count;
          if(tab[count] != resi)   --count;
          
          ratio = ratio * Bint(10) + Bint(count);
          if(count) resi = resi - tab[count];
          
          if(i>0) resi = resi * Bint(10) + Bint(n[i-1]-'0');
        }
      
      if(resi.s!=0) resi.s = s;
      
      return resi;
    }
}
 
bool Bint::operator<(const Bint& b) const
{
  if(s<b.s) return true;
  if(s==-1 && b.s==-1 && !lt(n, b.n) && n!=b.n) return true;
  if(s==1 && b.s==1 && lt(n, b.n)) return true;
  return false;
}
 
ostream& operator<<(ostream& os, const Bint& b)
{
  if(b.s == 0) {os << "0"; return os;}
  if(b.s == -1) os << "-";
  string t; t.assign(b.n.rbegin(), b.n.rend()); 
  os << t;
  return os;
}
 
istream& operator>>(istream& is, Bint& b)
{
  string t; is >> t;
  b = Bint(t);
  return is;
}
 
long long Bint::toLong() const
{
  return s*stringToLong(n);
}
 
Bint Bint::Bpow(int p) const
{
  if(p < 0) {cerr << "Negative power not implemented error: Bpow("<<*this<<","<<p<<")\n"; exit(1);}
  if(p == 0)
  {
    if(*this==0) {cerr << "Undefined Bpow(0,0) error\n"; exit(1);}
    else return 1;
  }
  if(*this == 1) return *this;
  
  int sign = 1;
  if(p%2==1) sign = this->s;
  
  Bint res(*this), a(1);
  while(p != 1)
    {
      if(p%2==0) {res*=res; p/=2;} // r^(2p) = (r^2)^p
      else       {--p; a*=res;}    // r^(2p+1) = r * r^(2p)
    }
  res *= a;
  res.s = sign;
  
  return res;
}
 
long long Bint::sumOfDigits() const
{
  int nsize = n.size();
  long long sum = 0;
  for(int i = 0; i<nsize; ++i) sum += n[i]-'0';
  return sum;
}
 
Bint Bint::factorial() const
{
  if(s <= 0) return 1;
  Bint fact(*this), i(1);
  for(; i != *this; ++i) fact *= i;
  return fact;
}
 
bool Bint::isPrime() const // All primes are of the form 6k-1 or 6k+1
{
  if(*this <= 10)
    {
      if(*this <= 1) return false; // 0 or 1
      if(*this <= 3) return true;  // 2 or 3
      if(*this == 5) return true;  // 5
      if(*this == 7) return true;  // 7
      return false;                // 4, 6, 8, 9 or 10
    }
  if(this->isEven())             return false; // *this is multiple of 2
  if(this->sumOfDigits()%3 == 0) return false; // *this is multiple of 3
  if(n[0] == '5')                return false; // *this is multiple of 5
 
  if(lt(this->n, maxlong)) // Use fast long long
    {
      long long test = this->toLong();
      long long num = 7;
      int w = 4;
      while(num*num <= test)
        {
          if(test%num==0) return false; // *this is multiple of num -> not prime
          num += w;                     // 11, 13, 17, 19, 23, 25, 29, 31, 35, ...
          w = 6-w;                      // num skips multiples of 2 and 3.
        }
    }
  else // Use never ending very slow Bint.
    {
//    cout << "Warning very slow \"isPrime\" function.\n";
      Bint num(7);
      int w = 4;
      while(num*num <= *this)
        {
          if(num.n[0]!='5' &&            // Skip num multiple of 5 mod test loops
             *this%num==0) return false; // *this is multiple of num -> not prime
          num += w;                      // 11, 13, 17, 19, 23, 25, 29, 31, 35, ...
          w = 6-w;                       // num skips multiples of 2 and 3.
        }
    }
 
  return true;
}
 
//==================================================================== END
