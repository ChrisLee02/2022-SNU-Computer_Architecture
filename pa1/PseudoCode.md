with array
1. 각 픽셀 위치에 대한 주변 평균값 구하기 -> 왼쪽, 위쪽, 좌측 위, 만약 범위 벗어날 경우 not count
2. 각 픽셀 위치에 대한 filter 구하기 -> pixel - filter // pixel < filter 일 경우 256 - filter 를 더한다.
3. row 별 min 으로 base 값 설정 후, delta 구하기
4. row 별로 delta 값의 최댓값을 기준으로 # of bits 결정
5. Base Value / # of bits / 0th ~ W-1th Delta Value 으로 인코딩 *단, Skip Delta Value when n(i) is zero
6. row 순서대로 encoded value 를 dst 에 기록, pad 0’s until it becomes a multiple of 8

without array  
선언 변수: filter, min, max, n(비트 수)
1. row 에 대한 루프
2. row 내에서 column 루프, 각 pixel 의 filter 값을 탐색, min, max 갱신
3. column 루프 종료 후 max - min 을 이용해 n값 결정, base 값은 min 값을 할당
![img_1.png](img_1.png)
4. base 값과 n 값을 dst에 쓴다.
5. n이 0이 아니라면, 다시 column 루프로 들어가 filter 값을 구하고, delta 값을 계산해 dst에 집어넣는다.
6. row 루프가 종료되면, padding 해준다.

![img.png](img.png)

