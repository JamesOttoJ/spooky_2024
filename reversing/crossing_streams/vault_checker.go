package main

import (
  "fmt"
  "log"
  "time"
  "math/rand"
)

func main() {
  rand.Seed(515797029933644)
  // fmt.Printf("%x\n", rand.Intn(256))
  fmt.Println("input text:")
  var w1 string
  n, err := fmt.Scanln(&w1)
  if err != nil {
    log.Fatal(err)
  }

  go f(w1, n)
  time.Sleep(1*time.Second)

  // fmt.Printf("number of items read: %d\n", n)
  // fmt.Printf("read line: %s-\n", w1)

}

func f(password string, n int) {
  key := rand.Intn(256)
  correct := []byte{0xa0, 0xa3, 0xe3, 0xe3, 0xb8, 0xaa, 0xa8, 0x8a, 0xe3, 0xa6, 0x8c, 0x92, 0xa1, 0xe0, 0x8c, 0xe7, 0x8c, 0xa1, 0xe0, 0xe7, 0xbf, 0x8c, 0xb4, 0xe3, 0xb4, 0xb6, 0xe4, 0xe4, 0xb6, 0xa1, 0xae}
  result := []byte(password)
  fmt.Printf("%x\n", key)
  for i, c := range password {
    result[i] = byte(c) ^ byte(key)
    fmt.Printf("%x %x\n", c, result[i])
  }
  for i := range correct {
    if (result[i] != correct[i]) {
      fmt.Printf("Incorrect.\n")
      return
    }
  }
  fmt.Printf("Correct.\n")
}
