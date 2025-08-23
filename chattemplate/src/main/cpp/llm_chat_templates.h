#ifndef POLYENGINEINFER_LLM_CHAT_TEMPLATES_H
#define POLYENGINEINFER_LLM_CHAT_TEMPLATES_H

#include <string>

namespace llm {

struct ChatTemplate {
  std::string jinja_statement;
  std::string bos_token;
  std::string eos_token;
};

namespace builtin {
const ChatTemplate &getBuiltInTemplateByName(const std::string &name);
}  // namespace builtin
}  // namespace llm

#endif  // POLYENGINEINFER_LLM_CHAT_TEMPLATES_H
