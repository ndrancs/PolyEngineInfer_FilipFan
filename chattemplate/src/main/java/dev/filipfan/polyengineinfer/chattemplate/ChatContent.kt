package dev.filipfan.polyengineinfer.chattemplate

import kotlinx.serialization.Serializable

@Serializable
internal data class ChatContent(
    val role: String,
    val content: String,
)
