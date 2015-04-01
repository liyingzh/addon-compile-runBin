#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

#include "v8.h"
#include <node.h>

using namespace v8;

void AddonCompile(const FunctionCallbackInfo<Value>& args) {

  Isolate* isolate = Isolate::GetCurrent();
  {
    String::Utf8Value source(args[0]);
    FILE* source_file = fopen(*source, "rb");
    if (source_file == NULL) {
      printf("Error loading file");
      return;
    }
    fseek(source_file, 0, SEEK_END);
    int size = ftell(source_file);
    rewind(source_file);

    char* chars = new char[size + 1];
    chars[size] = '\0';
    for (int i = 0; i < size;) {
      int read = static_cast<int>(fread(&chars[i], 1, size - i, source_file));
      if (read < 0) {
        printf("Failed to read %s\n", *source);
        return;
      }
      i += read;
    }
    fclose(source_file);
    Local<String> source_str = String::NewFromUtf8(isolate, chars);

    const char* v8_flag ="--nolazy --log_code --noharmony_shipping "
                         "--nologfile_per_isolate --serialize_toplevel";
    v8::V8::SetFlagsFromString(v8_flag, strlen(v8_flag));

    ScriptCompiler::Source script_source(source_str, ScriptOrigin(v8::Undefined(isolate)));
    ScriptCompiler::CompileUnbound(isolate, &script_source,
                                   v8::ScriptCompiler::kProduceCodeCache);

    if (!script_source.GetCachedData()) {
      printf("Error executing file");
      return;
    }
    if (script_source.GetCachedData()->rejected) {
      printf("Cache rejected");
      return;
    }

    int length = script_source.GetCachedData()->length;
    uint8_t* cache = new uint8_t[length];
    memcpy(cache, script_source.GetCachedData()->data, length);
    String::Utf8Value cache_file(args[1]);
    FILE* fp = fopen(*cache_file, "wb");
    fwrite(cache, 1, length, fp);
    fclose(fp);
  }

  Handle<String> success = String::NewFromUtf8(isolate, "compile successfully");
  args.GetReturnValue().Set(success);

}

void Init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "addonCompile", AddonCompile);
}

NODE_MODULE(compile, Init)
