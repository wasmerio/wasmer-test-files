#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct wasm_byte_vec_t {
  size_t size;
  const uint8_t* data;
} wasm_byte_vec_t;

__attribute__((import_module("wasm_c_api_v0"),
               import_name("wasm_engine_new"))) extern int32_t
wasm_engine_new(void);

__attribute__((import_module("wasm_c_api_v0"),
               import_name("wasm_store_new"))) extern int32_t
wasm_store_new(int32_t engine);

__attribute__((import_module("wasm_c_api_v0"),
               import_name("wasm_module_validate"))) extern int32_t
wasm_module_validate(int32_t store, const wasm_byte_vec_t* bytes);

__attribute__((import_module("wasm_c_api_v0"),
               import_name("wasm_module_new"))) extern int32_t
wasm_module_new(int32_t store, const wasm_byte_vec_t* bytes);

int main(void) {
  static const uint8_t empty_wasm[] = {0x00, 0x61, 0x73, 0x6d,
                                       0x01, 0x00, 0x00, 0x00};
  wasm_byte_vec_t bytes = {
      .size = sizeof(empty_wasm),
      .data = empty_wasm,
  };

  int32_t engine = wasm_engine_new();
  if (engine == 0) {
    fprintf(stderr, "wasm_engine_new returned null\n");
    return 10;
  }

  int32_t store = wasm_store_new(engine);
  if (store == 0) {
    fprintf(stderr, "wasm_store_new returned null\n");
    return 11;
  }

  if (!wasm_module_validate(store, &bytes)) {
    fprintf(stderr, "wasm_module_validate rejected the embedded module\n");
    return 12;
  }

  int32_t module = wasm_module_new(store, &bytes);
  if (module == 0) {
    fprintf(stderr, "wasm_module_new returned null\n");
    return 13;
  }

  puts("wasm-c-api ok");
  return 0;
}
