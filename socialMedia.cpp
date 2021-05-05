#include <cstdio>

int gcd(int a, int b){
    if(b==0)
        return a;
    if(a<b)
        return gcd(b,a);

    return gcd(b,a%b);
}

int lcm(int a, int b){
    return a*b/gcd(a,b);
}

int main(){
    printf("%d\n", gcd(12*3, 12*5));
    printf("%d\n", gcd(12*7, 12*5));
    printf("%d\n", gcd(12*8, 12*4));
    printf("%d\n", gcd(12*10, 12*5));

    printf("%d\n", lcm(12*3, 12*5));
    printf("%d\n", lcm(12*7, 12*5));
    printf("%d\n", lcm(12*8, 12*4));
    printf("%d\n", lcm(12*10, 12*5));

    return 0;
}