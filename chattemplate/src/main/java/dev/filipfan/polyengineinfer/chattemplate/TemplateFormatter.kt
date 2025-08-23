package dev.filipfan.polyengineinfer.chattemplate

import kotlinx.serialization.json.Json

class TemplateFormatter(template: BuiltInTemplates) {
    companion object {
        init {
            System.loadLibrary("chat-template-formatter")
        }
    }

    private val nativeHandle: Long

    init {
        nativeHandle = initChatTemplate(template.modelName)
        if (nativeHandle == 0L) {
            throw IllegalStateException("Failed to initialize template formatter")
        }
    }

    fun formatSystemContent(systemPrompt: String): String = formatContentInternal("system", systemPrompt)

    fun formatContent(prompt: String): String = formatContentInternal("user", prompt)

    private fun formatContentInternal(role: String, prompt: String): String {
        val content = ChatContent(role = role, content = prompt)
        val jsonString = Json.encodeToString(content)
        return applyChatTemplate(nativeHandle, jsonString)
    }

    fun release() {
        if (nativeHandle != 0L) {
            destroyChatTemplate(nativeHandle)
        }
    }

    private external fun initChatTemplate(model: String): Long
    private external fun destroyChatTemplate(handle: Long)
    private external fun applyChatTemplate(handle: Long, contentJson: String): String
}
