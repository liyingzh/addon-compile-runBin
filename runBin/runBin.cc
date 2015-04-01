#include <stdio.h>
#include <string.h>

#include <v8.h>
#include <node.h>

using namespace v8;

void AddonRunBin(const FunctionCallbackInfo<Value>& args) {
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  for (int i = 0; i < args.Length(); i++) {
    String::Utf8Value bin_source(args[i]);
    FILE* source_file = fopen(*bin_source, "rb");
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
        printf("Failed to read %s\n", *bin_source);
        return;
      }
      i += read;
    }
    fclose(source_file);

    if (chars == NULL) return;
    uint8_t* cache_data = new uint8_t[size];
    memcpy(cache_data, chars, size);
    delete[] chars;

    const char* v8_flag ="--serialize_toplevel";
    v8::V8::SetFlagsFromString(v8_flag, strlen(v8_flag));

    ScriptCompiler::CachedData* cache = new ScriptCompiler::CachedData(
        cache_data, size, ScriptCompiler::CachedData::BufferNotOwned);
    Handle<String> source = String::NewFromUtf8(isolate, "");
    ScriptCompiler::Source cached_source(source, cache);
    Local<UnboundScript> script = ScriptCompiler::CompileUnbound(
        isolate, &cached_source, ScriptCompiler::kConsumeCodeCache);

    if (cache->rejected) {
      printf("Cache rejected");
      return;
    }
    script->BindToCurrentContext()->Run();
  }
}

void Init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "addonRunBin", AddonRunBin);
}

NODE_MODULE(runBin, Init)
