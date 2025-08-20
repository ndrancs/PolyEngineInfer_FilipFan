package dev.filipfan.polyengineinfer.chattemplate

import androidx.test.ext.junit.runners.AndroidJUnit4
import org.junit.After
import org.junit.Assert.assertEquals
import org.junit.Test
import org.junit.runner.RunWith

@RunWith(AndroidJUnit4::class)
class TemplateFormatterTest {

    private var formatter: TemplateFormatter? = null

    @After
    fun tearDown() {
        formatter?.release()
        formatter = null
    }

    @Test
    fun formatSystemContentWithLlama3() {
        formatter = TemplateFormatter(BuiltInTemplates.LLAMA_3)
        val result = formatter!!.formatSystemContent("You are a helpful assistant")
        val expected = """
            <|start_header_id|>system<|end_header_id|>
            You are a helpful assistant<|eot_id|>
        """.trimIndent()
        assertEquals(
            expected,
            result.lines().filter { it.isNotEmpty() }
                .joinToString("\n"),
        )
    }

    @Test
    fun formatContentWithLlama3() {
        formatter = TemplateFormatter(BuiltInTemplates.LLAMA_3)
        val result = formatter!!.formatContent("Hello, world!")

        val expected = """
            <|start_header_id|>user<|end_header_id|>
            Hello, world!<|eot_id|>
            <|start_header_id|>assistant<|end_header_id|>
        """.trimIndent()
        assertEquals(
            expected,
            result.lines().filter { it.isNotEmpty() }
                .joinToString("\n"),
        )
    }

    @Test
    fun formatSystemContentWithGemma() {
        formatter = TemplateFormatter(BuiltInTemplates.GEMMA)
        val result = formatter!!.formatSystemContent("You are a helpful assistant")
        // Gemma's instruction-tuned models are designed to work with only two roles: user and model.
        // See https://ai.google.dev/gemma/docs/core/prompt-structure#system-instructions.
        val expected = """
            <start_of_turn>user
            You are a helpful assistant<end_of_turn>
        """.trimIndent()
        assertEquals(
            expected,
            result.lines().filter { it.isNotEmpty() }
                .joinToString("\n"),
        )
    }

    @Test
    fun formatContentWithGemma() {
        formatter = TemplateFormatter(BuiltInTemplates.GEMMA)
        val result = formatter!!.formatContent("Hello, world!")
        val expected = """
            <start_of_turn>user
            Hello, world!<end_of_turn>
            <start_of_turn>model
        """.trimIndent()
        assertEquals(
            expected,
            result.lines().filter { it.isNotEmpty() }
                .joinToString("\n"),
        )
    }
}
