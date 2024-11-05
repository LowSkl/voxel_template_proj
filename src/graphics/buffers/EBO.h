#pragma once

#include "../SneakyThings.h"

class EBO
{
    unsigned int m_UUID  = 0;
    unsigned int m_count = 0;

    void finalize();

public:
    EBO(const void* data, const size_t size, const Usage usage = Usage::Static);
    ~EBO();

    EBO(const EBO&     ) = delete;
    EBO(      EBO&& ebo) noexcept;

    EBO& operator=(const EBO&     ) = delete;
    EBO& operator=(      EBO&& ebo) noexcept;

    void bind() const;
    static void unbind();

    void reload(const void* data, const size_t size, const Usage usage = Usage::Static);

    unsigned int get_UUID()  const { return m_UUID;  }
    unsigned int get_count() const { return m_count; }
};