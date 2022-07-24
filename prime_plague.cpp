#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>
#include <string>
#include <unordered_set>
#include <limits.h>
#include <algorithm>

// so that I dont have to type out long long int every time
typedef long long int big;

// Absolute value function
#define abs(X) ((X) < 0 ? -(X) : (X))

// Given an (x, y) position, what is the number at that position
inline big getNum(big x, big y){
    if (-x >= abs(y))   return (4*x*x) - (x) - (y)   + 1;
    if ( y >= abs(x))   return (4*y*y) - (x) - (y)   + 1;
    if (-y >= abs(x))   return (4*y*y) + (x) - (3*y) + 1;
                        return (4*x*x) - (3*x) + (y) + 1;
}

// gets the closest odd perfect square but the sqrt of that
// Ex: 8 is closest to 9, so it will evaluate to 3 (sqrt(9))
inline big closestOddSquareRoot(big n){
    big close = (big)sqrtl(n);
    return (close - (close % 2) + 1);
}

// An (x, y) coordinate position
struct pos{
    big x;
    big y;
};

// Given a number what is its position
pos getPos(big n){
    pos p;
    big sqrt_close = closestOddSquareRoot(n);
    big close = sqrt_close*sqrt_close;
    p.x = (sqrt_close - 1)/2;
    p.y = -p.x;
    if (n < close){
        if (n >= (close - sqrt_close + 1)){
            p.x += (n - close);
            return p;
        }
        if (n > ((sqrt_close-1) * (sqrt_close-1))){
            p.x += (1 - sqrt_close);
            p.y += (close - n - sqrt_close + 1);
            return p;
        }
        p.x = 1 - p.x;
        p.y = -p.y;
    } else {
        if (n <= (close + sqrt_close)){
            p.x += 1;
            p.y += (n-close-1);
            return p;
        }
        p.y = -p.y + 1;
        p.x += 1 - (n - (close + sqrt_close + 1));
    }
    return p;
}
// File found from this website
// https://www.gutenberg.org/files/58225/58225-h/58225-h.htm
void readPrimesFromFile(std::unordered_set<big>* primes, int num){
    std::ifstream primeFile;
    primeFile.open(std::to_string(num) + ".txt");
    std::string line;
    while (getline(primeFile, line)) {
        primes->insert(std::stoll(line));
    }
    primeFile.close();
}

// Given an integer it detects whether it is prime or not
bool isPrime(big num) {
    if(num <= 1)  return false;
    if (num <= 3) return true; 
    
    int range = sqrtl(num)+1;
    if (num % 2 == 0 || num % 3 == 0) 
        return false; 
    

    for (int i = 5; i <= range; i += 6) 
        if (num % i == 0 || num % (i + 2) == 0) 
            return false; 
    
    return true;
}

// Data for a specific infection tree
struct data{
    big seed;
    size_t length;
};

int main(void){
    // Going through each file
    for (int i = 401; i <= 500; i++){
        std::unordered_set<big> primes;

        std::cout << "Beginning reading from file: " << i << "\n";
        readPrimesFromFile(&primes, i);

        std::vector<data> infections;

        // Taking out the infection that starts at 2 manually because 2 being even causes edge cases that dealt with in a loop would be inefficient
        if (i == 1){
            infections.push_back({2, 8});
            primes.erase(2);
            primes.erase(3);
            primes.erase(11);
            primes.erase(29);
            primes.erase(13);
            primes.erase(31);
            primes.erase(53);
            primes.erase(59);
        }
        
        while (primes.size() > 0){
            std::vector<pos> infected;
            std::unordered_set<big> dead;
            // adding a prime to the infected list
            infected.push_back(getPos(*primes.begin()));
            // While there are still infected number left
            while(infected.size() > 0){
                for (int y = infected[0].y-1; y <= infected[0].y+1; y += 2){
                    for (int x = infected[0].x-1; x <= infected[0].x+1; x += 2){
                        big num = getNum(x, y);
                        // if the number is not dead and it is prime infect it
                        if ((dead.find(num) == dead.end()) && (primes.find(num) != primes.end() || isPrime(num))){
                            infected.push_back({x, y});
                        }
                    }
                }
                // the current number has now died
                dead.insert(getNum(infected[0].x, infected[0].y));
                infected.erase(infected.begin());
            }
            // adding the infection tree by the minumum number of the infection tree
            big min = LLONG_MAX;
            for (const auto& d : dead){
                if (d < min) min = d;
                primes.erase(d); // erasing the number from the primes list
            }
            infections.push_back({min, dead.size()});
            //std::cout << min << " : " << dead.size() << '\n';



        }
        // sorting all of the infections for the one that infects the most
        std::sort(infections.begin(), infections.end(), [](const data &a, const data &b){
            if (a.length == b.length) return a.seed < b.seed;
            return a.length > b.length;
        });

        // Outputting the information to the console
        std::cout << infections[0].seed << " : " << infections[0].length << "\n\n";
        //std::cout << getPos(infections[0].seed).x << ", " << getPos(infections[0].seed).y << std::endl;
        //std::cout << getNum(getPos(infections[0].seed).x, getPos(infections[0].seed).y) << std::endl;
    }
}