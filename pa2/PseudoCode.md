1. If `|x|` < `|y|`, swap the operands.

2. Now we extend M<sub>x</sub> and M<sub>y</sub> to include the extra three bits (G, R, and S bits) after the `p`-bit fraction. Those bits are initialized to zeroes. And then we shift right M<sub>y</sub> by `d` = E<sub>x</sub> - E<sub>y</sub> bits. From the bits shifted out beyond the `p` bits, we set the guard bit to the most significant bit (i.e., (`p`+1)-th bit), the round bit to the next most significant bit (i.e., (`p`+2)-th bit), and sticky bit to the OR of the rest.

3. If the signs of `x` and `y` are same, compute a preliminary significand `M` by adding M<sub>y</sub> to M<sub>x</sub> using the integer arithmetic. If the signs of `x` and `y` are different, subtract M<sub>y</sub> from M<sub>x</sub>. Also, set the exponent `E` and the sign bit `S` of the result such that `E` = E<sub>x</sub> and `S` = S<sub>x<sub>.

4. Normalize the preliminary significand `M`. Whenever `M` is shifted left, those G, R, and S bits are shifted together as if they were part of the normal fraction bits, filling the sticky bit with zero. Likewise, whenever `M` is shifted right during the normalization step, G, R, and S bits are updated as follows (assuming `b` is the new bit shifted out beyond the `p` bits):
   ```
      S_bit <- S_bit | R_bit
      R_bit <- G_bit
      G_bit <- b
   ```
   In any case, the exponent of the result, `E`, should be adjusted accordingly.

5. The next step is to perform rounding. The main role of the guard bit is to fill in the least-significant bit (`p`-th bit) of the result when necessary. Depending on the situation, the original guard bit in the step 3 may have moved to the left or right direction during normalization. But in any case, the role of the guard bit is over after the normalization step. Hence, we adjust the round bit and the sticky bit as follows:
   ```
      S_bit <- S_bit | R_bit
      R_bit <- G_bit
   ```

   This is because, when it comes to the rounding, what really matters is the discarded bits. We set the round bit to the previous guard bit because it is the first bit among the discarded bits. We also update the sticky bit to include the status of the previous round bit.

   Using the updated round bit and the sticky bit, we perform rounding according to the round-to-even mode: if (R_bit &and; S_bit) &or; (L_bit &and; R_bit), round up, otherwise round down. (Here, L_bit means the last bit of the fraction, i.e., `p`-th bit.)

6. Perform the normalization and rounding again if necessary.

7. Encode the result into binary numbers according to the `SFP16` format.
* For special cases where +inf/-inf, +0/-0, and +NaN/-NAN are involved, the return value should be the same as the one shown in the following table.

```
     y:    +inf   -inf    nan    zero   other
 x:     
  +inf     +inf    nan    nan    +inf   +inf
  -inf      nan   -inf    nan    -inf   -inf
   nan      nan    nan    nan     nan    nan
   zero    +inf   -inf    nan     zero
   other   +inf   -inf    nan