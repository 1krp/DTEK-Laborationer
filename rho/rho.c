// rho.c
// gcc -O2 -std=c11 rho.c -lgmp -o rho_gmp
#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

// ---- Progress tuning -------------------------------------------------------
#ifndef RHO_PROGRESS       // set to 0 to disable at compile time
#define RHO_PROGRESS 1
#endif
#ifndef RHO_ITERS_LOG2     // check the clock every 2^RHO_ITERS_LOG2 iterations
#define RHO_ITERS_LOG2 22  // ~1,048,576 iterations between time checks
#endif
#define RHO_ITERS_MASK ((1ULL << RHO_ITERS_LOG2) - 1ULL)
// ---------------------------------------------------------------------------

static void rand_mod(mpz_t r, const mpz_t n, gmp_randstate_t st) {
    mpz_urandomm(r, st, n);
    if (mpz_cmp_ui(r, 0) == 0) mpz_set_ui(r, 1);
}

static void f(mpz_t out, const mpz_t x, const mpz_t c, const mpz_t n) {
    // out = (x*x + c) % n
    mpz_mul(out, x, x);
    mpz_add(out, out, c);
    mpz_mod(out, out, n);
}

static void rho(mpz_t factor, const mpz_t n, gmp_randstate_t st) {
    if (mpz_even_p(n)) { mpz_set_ui(factor, 2); return; }

    mpz_t x, y, c, d, tmp;
    mpz_inits(x, y, c, d, tmp, NULL);

#if RHO_PROGRESS
    const time_t t_start = time(NULL);
    time_t t_last = t_start;
    unsigned long long iters = 0;    // number of inner-loop iterations
    unsigned long long restarts = 0; // number of parameter restarts
#endif

    do {
        rand_mod(c, n, st);
        rand_mod(x, n, st);
        mpz_set(y, x);
        mpz_set_ui(d, 1);
#if RHO_PROGRESS
        restarts++;
#endif

        while (mpz_cmp_ui(d, 1) == 0) {
            f(x, x, c, n);
            f(y, y, c, n);
            f(y, y, c, n);
            mpz_sub(tmp, x, y);
            mpz_abs(tmp, tmp);
            mpz_gcd(d, tmp, n);

#if RHO_PROGRESS
            // Increment step counter; check the clock every ~2^RHO_ITERS_LOG2 steps
            iters++;
            if ((iters & RHO_ITERS_MASK) == 0) {
                time_t now = time(NULL);
                if (now != t_last) {
                    double elapsed = (double)(now - t_start);
                    double speed = elapsed > 0.0 ? (double)iters / elapsed : 0.0;
                    fprintf(stderr,
                            "\r[rho] iters=%llu  restarts=%llu  elapsed=%.0fs  ~%.0f iters/s",
                            (unsigned long long)iters,
                            (unsigned long long)restarts - 1ULL,
                            elapsed, speed);
                    fflush(stderr);   // flush only on the ~1s tick
                    t_last = now;
                }
            }
#endif
        }
        if (mpz_cmp(d, n) == 0) mpz_set_ui(d, 1); // retry with new parameters
    } while (mpz_cmp_ui(d, 1) == 0);

#if RHO_PROGRESS
    {
        time_t now = time(NULL);
        double elapsed = (double)(now - t_start);
        double speed = elapsed > 0.0 ? (double)iters / elapsed : 0.0;
        fprintf(stderr,
                "\r[rho] done: iters=%llu  restarts=%llu  elapsed=%.0fs  ~%.0f iters/s\n",
                (unsigned long long)iters,
                (unsigned long long)restarts - 1ULL,
                elapsed, speed);
    }
#endif

    mpz_set(factor, d);
    mpz_clears(x, y, c, d, tmp, NULL);
}

static int is_probable_prime(const mpz_t n) {
    // 25 rounds Miller–Rabin (probability of error negligible)
    return mpz_probab_prime_p(n, 25) > 0;
}

static void factor(mpz_t n) {
    if (mpz_cmp_ui(n, 1) == 0) return;
    if (is_probable_prime(n)) { gmp_printf("One fraction is -> %Zd\n", n); return; }

    gmp_randstate_t st;
    gmp_randinit_mt(st);
    gmp_randseed_ui(st, (unsigned)time(NULL));

    mpz_t d, m;
    mpz_inits(d, m, NULL);
    rho(d, n, st);
    mpz_divexact(m, n, d);

    factor(d);
    factor(m);

    mpz_clears(d, m, NULL);
    gmp_randclear(st);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        return 1;
    }
    mpz_t n;
    mpz_init_set_str(n, argv[1], 10); // read base-10 integer
    factor(n);
    mpz_clear(n);
    return 0;
}
