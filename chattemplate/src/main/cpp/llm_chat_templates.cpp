#include "llm_chat_templates.h"

#include <string>
#include <unordered_map>

using llm::ChatTemplate;

namespace /* anonymous */ {

const ChatTemplate llama3 = {.jinja_statement = R"TEMPLATE(
{% if messages[0]['role'] == 'system' %}
    {% set offset = 1 %}
{% else %}
    {% set offset = 0 %}
{% endif %}

{{ bos_token }}
{% for message in messages %}
    {% if (message['role'] == 'user') != (loop.index0 % 2 == offset) %}
        {{ raise_exception('Conversation roles must alternate user/assistant/user/assistant/...') }}
    {% endif %}

    {{ '<|start_header_id|>' + message['role'] + '<|end_header_id|>\n\n' + message['content'] | trim + '<|eot_id|>' }}
{% endfor %}

{% if add_generation_prompt %}
    {{ '<|start_header_id|>' + 'assistant' + '<|end_header_id|>\n\n' }}
{% endif %}
)TEMPLATE",
                             .bos_token = "<|begin_of_text|>",
                             .eos_token = "<|end_of_text|>"};

const ChatTemplate gemma = {.jinja_statement = R"TEMPLATE(
{% if messages[0]['role'] == 'system' %}
    {% set _ = messages.update({0: {'role': 'user', 'content': messages[0]['content']}}) %}
{% endif %}

{% for message in messages %}
    {% if (message['role'] == 'user') != (loop.index0 % 2 == 0) %}
        {{ raise_exception('Conversation roles must alternate user/assistant/user/assistant/...') }}
    {% endif %}

    {% if message['role'] == 'assistant' %}
        {% set role = 'model' %}
    {% else %}
        {% set role = message['role'] %}
    {% endif %}

    {{ '<start_of_turn>' + role + '\n' + message['content'] | trim + '<end_of_turn>\n' }}
{% endfor %}

{% if add_generation_prompt %}
    {{'<start_of_turn>model\n'}}
{% endif %}
)TEMPLATE",
                            .bos_token = "<bos>",
                            .eos_token = "<eos>"};

const auto &get_template_map() {
  static const std::unordered_map<std::string, const llm::ChatTemplate &>
      built_in_templates = {{"llama_3", llama3}, {"gemma", gemma}};
  return built_in_templates;
}

}  // anonymous namespace

namespace llm::builtin {

const ChatTemplate &getBuiltInTemplateByName(const std::string &name) {
  try {
    return get_template_map().at(name);
  } catch (const std::out_of_range &e) {
    throw std::out_of_range("Unknown built-in template name: " + name);
  }
}

}  // namespace llm::builtin
