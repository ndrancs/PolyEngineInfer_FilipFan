#include <android/log.h>
#include <jni.h>

#include <minja/chat-template.hpp>
#include <nlohmann/json.hpp>

#include "llm_chat_templates.h"

#define TAG "jni_template_formatter.cpp"
#define LOGi(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGe(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

#define JNI_METHOD(METHOD_NAME) \
  Java_dev_filipfan_polyengineinfer_chattemplate_TemplateFormatter_##METHOD_NAME

// Helper function to throw a Java exception from C++.
void throwJavaException(JNIEnv *env, const char *className,
                        const char *message) {
  jclass exClass = env->FindClass(className);
  if (exClass != nullptr) {
    env->ThrowNew(exClass, message);
  }
}

extern "C" JNIEXPORT jlong JNICALL JNI_METHOD(initChatTemplate)(JNIEnv *env,
                                                                jobject,
                                                                jstring model) {
  auto name_array = env->GetStringUTFChars(model, nullptr);
  std::string name = name_array;
  env->ReleaseStringUTFChars(model, name_array);
  auto &built_in_template = llm::builtin::getBuiltInTemplateByName(name);
  auto tmpl = new minja::chat_template(built_in_template.jinja_statement,
                                       built_in_template.bos_token,
                                       built_in_template.eos_token);
  return reinterpret_cast<jlong>(tmpl);
}

extern "C" JNIEXPORT void JNICALL JNI_METHOD(destroyChatTemplate)(
    JNIEnv * /* env */, jobject /* this */, jlong handle) {
  if (handle == 0) {
    return;
  }

  auto *tmpl = reinterpret_cast<minja::chat_template *>(handle);
  delete tmpl;
}

extern "C" JNIEXPORT jstring JNICALL JNI_METHOD(applyChatTemplate)(
    JNIEnv *env, jobject /* this */, jlong handle, jstring json_content) {
  if (handle == 0) {
    throwJavaException(env, "java/lang/IllegalArgumentException",
                       "Invalid native handle");
    return {};
  }

  const char *json_chars = env->GetStringUTFChars(json_content, nullptr);
  std::string json_content_str = json_chars;
  env->ReleaseStringUTFChars(json_content, json_chars);

  json chat_content_json = nlohmann::json::parse(json_content_str);
  std::string role = chat_content_json["role"];
  std::string content = chat_content_json["content"];

  minja::chat_template_inputs inputs;
  inputs.messages =
      nlohmann::json::array({{{"role", role}, {"content", content}}});
  inputs.add_generation_prompt = !(role == "system");

  minja::chat_template_options options;
  options.use_bos_token = false;
  options.use_eos_token = false;

  auto *tmpl = reinterpret_cast<minja::chat_template *>(handle);
  std::string result = tmpl->apply(inputs, options);
  return env->NewStringUTF(result.c_str());
}
