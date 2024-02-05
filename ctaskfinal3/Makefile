CC = gcc
CFLAGS = -Wall -Wextra
TARGET = my_program
SRCS = 00_main.c 01_rectarea.c 02_peri.c 03_psquare.c 04_sqarea.c 05_avg.c 06_subjsum.c 07_resistance.c 08_power.c 09_kirsum.c 10_xval.c 11_larg.c 12_pos.c 13_larg3.c 14_grade.c 15_kircheck.c 16_dl.c 17_greet.c 18_sum.c 19_even.c 20_20nums.c 21_neven.c 22_30odd.c 23_eveninrange.c 24_evenrev.c 25_nnumrev.c 26_nevenrev.c 27_series1.c 28_series2.c 29_3mul.c 30_allmul.c 31_p1.c 32_p2.c 33_p3.c 34_p4.c 35_p5.c 36_p6.c 37_fact.c 38_factors.c 39_prime.c 40_revnum.c 41_fibanocci.c 42_gcd.c 43_billing.c 44_palindrome.c 45_bcd.c 46_asc.c 47_numtozero.c 48_nprimenums.c 49_hexToDecimal.c 50_decimalToHex.c
OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

