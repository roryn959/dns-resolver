#include "include/Resolver.h"

// dig @127.0.0.1 -p 8080 example.com +noedns
// 198.41.0.4

int main() {
    Resolver resolver;
    resolver.run();
}