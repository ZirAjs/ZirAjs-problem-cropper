# ZirAjs-problem-cropper

## 개요


문제 이미지 파일 또는 pdf파일이 있을 때 그 안의 이미지를 손쉽게 잘라낼 수 있도록 도와주는 프로그램입니다. 프로그램을 개발한 동기는 기존에 모의고사 문제를 캡쳐하는 과정에서 화질이 떨어지는 것이 많이 불편했던 경험 때문입니다. 따라서 저는 *pdf를 캡쳐하는 방식*보다 더 유용한 기능을 아래와 같이 제공합니다.

- **화질 깨짐 걱정 X**
- pdf를 자동으로 이미지 전환
- 캡쳐 순서대로 이미지를 폴더에 저장
- (추가예정) ~~캡쳐한 이미지를 hwp파일로 저장~~

이 기능을 잘 활용하면 작업을 더 편하게 할 수 있을 것입니다.정확한 실행 화면을 보려면 [블로그](https://zirajs.tistory.com/12)를 확인해주세요

예를 들어 폴더명\[고2 2021년 10월 학력평가 - 물리1\] 아래 시험지 pdf를 둔 이후 캡쳐를 완료하고 저장을 하면 "고2 2021년 10월 학력평가 - 물리1_1", "고2 2021년 10월 학력평가 - 물리1_2" ... "고2 2021년 10월 학력평가 - 물리1_20" 이처럼 이미지로 저장되서 관리가 쉬울 것입니다. (hwp로 저장하면 훨신 더 간편할 예정이고요.)

## 설치 방법

Release에서 다운받으실 수 있습니다.

다운받은 이후 pdf를 이미지로 변환해서 읽어오는 기능을 사용하기 위해서는 추가 프로그램 설치가 필요합니다. ZirAjs-problem-cropper는 pdf→png 변환에 xpdf라는 프로그램을 이용합니다. 따라서 [xpdf홈페이지](https://www.xpdfreader.com/download.html)에서 "Xpdf command line tools" 중 'windows'을 다운받고, 압축을 해제한 뒤 `bin64`의 `pdftopng.exe`을 ZirAjs-problem-cropper폴더에 넣으시면 됩니다.

## 사용법

1. 메뉴탭의 \[file\]>\[Open Image form ~\]를 클릭해서 이미지/pdf을 엽니다. 이때 여러개의 이미지, pdf파일을 열 수 있습니다.
2. 화면이 로드되면 화면을 크기에 맞게 조정하고(+,- 버튼 사용가능) 우측 도구상자의 \[select\]버튼을 누릅니다.
3. \[select\]버튼을 누르면 캡쳐모드에 들어가게 됩니다. 이 상태에서 원하는 영역을 그리시면 됩니다(마우스를 누른 곳과 마우스를 놓은 곳을 꼭짓점으로 하는 직사각형이 생깁니다).
4. 되돌리기와 원복하기를 이용해서 잘못 그린 부분을 취소할 수 있습니다.
5. \[select\]할 때 나오는 초록색 점은 마우스가 눌린 위치를 표시한 것이기에 큰 신경을 쓰실 필요는 없습니다.
6. 선택을 모두하셨다면 하단의 \[Save Cropped Problems\]버튼을 누르면 저장할 수 있습니다. 저장되는 파일 이름은 `(1.에서 선택한 파일이 있는 폴더의 이름)_(번호).png`로 저장됩니다.

## 단축키 목록
|기능|단축키|
|------|---|
|영역 선택 모드|`T`|
|영역 선택 모드 취소|`esc`|
|되돌리기|`Ctrl+Z`|
|복원하기|`Shift+Ctrl+Z` 또는 `Ctrl+Y`|

## 기타

- pdf 페이지의 수가 많거나 di가 높다면 처리하는데 시간이 조금 걸릴 수 있습니다.
- 화질이 높다보니 문서작업 시 고화질 이미지를 많이 넣게 되면 랙이 걸릴수도 있는데, 그럴때는 이미지 압축 기능을 이용하시는 것을 추천드립니다.(한글, 워드에는 이 기능이 내장되어있음)

필요한 곳에 써주시면 감사하겠습니다.
