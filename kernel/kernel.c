void foo() {}

void main() {
  int a = 123;
  int b = 0x1234;

  char* name = "Steve";

  char* video_mem = (char*)0xb8000;
  *video_mem = 'X';
  *(video_mem + 2) = 'Y';
}
