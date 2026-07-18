# io.h で作った関数をNimに教えてあげる（importする感覚）
proc clear() {.importc: "clear".}
proc print(str: cstring) {.importc: "print".}

# コマンドを判定するNimの関数
proc executeCommand(cmd: cstring) {.exportc: "executeCommand".} =
  if cmd == "clear":
    clear()
  elif cmd == "hello":
    print("Hello! This is Hama OS, powered by Nim!\n")
  else:
    print("Unknown command!\n")
