#include <iostream>
#include <ostream>

class Buffer 
{
public:
    /** The number of bytes allocated */
    long size_;
    /** The actual data */
    char* data_;
    /** The number of String objects referring to this Buffer */
    long ref_count_;
    /** The number of bytes used in the array */
    long used;

    explicit Buffer(long size)
        : size_(size), ref_count_(1), used(0)
    {
        data_ = new char[size_];
        used = 0;
    }

    explicit Buffer(const char* input)
        : Buffer(strlen(input))
    {
        strcpy(data_, input);
        used = strlen(input);
    }

    ~Buffer()
    {
        std::cout << "Destroying buffer " << data_ << std::endl;
        delete[] data_;
    }

    void reserve(size_t n)
    {
        n = std::max(static_cast<size_t>(1.5 * size_), n);
        auto new_data = new char[n];
        std::copy(data_, data_ + used, new_data);

        delete[] data_;
        data_ = new_data;
    }

};

class String
{
    /** The buffer. Might be null if move operator / constructor has been called. */
    Buffer* buffer = nullptr;

public:
    String()
        : String(10)
    {
    }

    explicit String(long n)
    {
        std::cout << "Default constructor with n = " << n << std::endl;
        buffer = new Buffer(n);
    }

    explicit String(const char* input)
    {
        std::cout << "const char* constructor" << std::endl;
        buffer = new Buffer(input);
    }

    // Copy constructor
    String(const String& o)
    {
        std::cout << "Copy constructor" << std::endl;
        buffer = o.buffer;
        buffer->ref_count_++;
    }

    // Move constructor
    String(String&& o) noexcept
    {
        std::cout << "Move constructor" << std::endl;
        std::swap(o.buffer, buffer);
    }

    // Copy operator
    String& operator=(const String& o)
    {
        std::cout << "Copy operator" << std::endl;
        String temp(o);
        std::swap(buffer, temp.buffer);
        return *this;
    }

/*    // Copy value operator
    String& operator=(String o)
    {
        std::cout << "Copy-value operator" << std::endl;
        std::swap(o.buffer, buffer);
        return *this;
    }*/

    // Move operator
    String& operator=(String&& o) noexcept
    {
        if (buffer && buffer->ref_count_ > 1)
            buffer->ref_count_--;
        else
            delete buffer;

        std::cout << "Move operator" << std::endl;
        buffer = o.buffer;
        o.buffer = nullptr;
        return *this;
    }

    ~String()
    {
        // Move operations may have taken the buffer out, so we'd rather check for nullness here
        if (!buffer)
            return;

        std::cout << "Destructor called on String " << buffer->data_ << std::endl;
        buffer->ref_count_--;

        if (buffer->ref_count_ == 0)
        {
            delete buffer;
        }
        else
        {
            std::cout << "Buffer " << buffer->data_ << " still has " << buffer->ref_count_ << " refs" << std::endl;
        }
    }

    void append(char c)
    {
        if (buffer->ref_count_ > 1)
        {
            buffer->ref_count_--;
            auto new_buffer = new Buffer(buffer->used);
            std::copy(buffer->data_, buffer->data_ + buffer->used, new_buffer->data_);
            new_buffer->used = buffer->used;
            buffer = new_buffer;
        }

        buffer->reserve(buffer->used + 1);
        buffer->data_[buffer->used++] = c;
    }

    void display(Buffer* to_display = nullptr)
    {
        if (!to_display)
            to_display = buffer;
            
        for(long i = 0; i < to_display->used; i++)
            std::cout << to_display->data_[i];

        std::cout << std::endl;
    }

    String operator+(const String& o) const
    {
        std::cout << "operator+" << std::endl;
        // Create a new string of the right size
        String result(o.buffer->used + buffer->used);
        strcpy(result.buffer->data_, buffer->data_);
        strcpy(result.buffer->data_ + buffer->used, o.buffer->data_);
        result.buffer->used = o.buffer->used + buffer->used;
        return result;
    }

    String& operator+=(const String& o)
    {
        std::cout << "operator+=" << std::endl;
        if (buffer->ref_count_ == 1)
        {
            strcpy(buffer->data_ + buffer->used, o.buffer->data_);
            buffer->used += o.buffer->used;
        }
        else
        {
            buffer->ref_count_--;
            std::cout << "operator+=: allocating a new buffer with " << buffer->data_ << std::endl;
            auto new_buffer = new Buffer(buffer->used + o.buffer->used);
            strcpy(new_buffer->data_, buffer->data_);
            strcpy(new_buffer->data_ + buffer->used, o.buffer->data_);
            new_buffer->used = buffer->used + o.buffer->used;
            buffer = new_buffer;
        }
        return *this;
    }
};

int main()
{
    String test;
    test.append('t');
    test.append('h');

    String test2 = test;
    test2.display();

    test2.append('h');
    test.display();
    test2.display();

    std::cout << "Assignment to auto" << std::endl;
    auto from_value = String("FROM_VALUE");
    from_value.display();

    std::cout << "Operator+" << std::endl;
    auto first = String("Thomas");
    auto second = String("Kowalski");
    auto sum = first + second;
    std::cout << "Sum: ";
    sum.display();

    std::cout << "Operator+=" << std::endl;
    auto result = first;
    result += second;
    result.display();

    std::cout << "Reusing the same variable" << std::endl;
    auto var = first; // Assign Thomas to var
    var = String("Hello, World!");

    // Check this buffer is going to be deleted
    auto var2 = String("THOMAS");
    var2 = String("THOMAS2");

    std::cout << "End of programme" << std::endl;
}