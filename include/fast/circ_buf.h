#ifndef CIRC_BUF_H
#define CIRC_BUF_H
#include <vector>

namespace sheep
{
/*
 * faster circular buffer implementation using a dynamic array
 */
template <class T> class CircBuf
{
  public:
    explicit CircBuf(const int size) : m_buf_size(size), m_occupation(0), m_override(false)
    {
        m_buf.reserve(m_buf_size);
    };

    /*
     * appends one element to the circular buffer
     * returns false once the buffer is full and doesn't append the given element, otherwise returns true and appends
     * the given element except if the override is set, then it'll always return true and override elements
     */
    bool push_back(T element)
    {
        bool success = false;
        // there is no space anymore
        if (m_occupation >= m_buf_size)
        {
            // override flag is set, so start overwriting
            if (m_override)
            {
                m_buf.at(m_current_element_index) = element;
                m_current_element_index++;
                success = true;
            }
            else
            {
                success = false;
            }
        }
        else
        {
            // there is still space for a new element
            m_buf.push_back(element);
            m_occupation++;
            success = true;
        }
        return success;
    };

    /*
     * returns the element at the current index
     * NOTE: the index wraps around
     */
    T at(int index)
    {
        while (index >= m_buf_size)
        {
            index = m_buf_size % index;
        }
        return m_buf.at(index);
    }

    /*
     * sets whether push_back should simply override the elements when the buffer is full
     */
    void set_override(bool override)
    {
        m_override = override;
    };

    /*
     * clears internal data array
     */
    /// IMPL: sort of a pseudo clear, simply releases all objects so they can be overwritten
    void clear()
    {
        m_override = true;
    };

    /*
     * allocate memory based on new size
     */
    void resize(const int size)
    {
        // new buffer size so that .at works correctly
        m_buf_size = size;
    };

    /*
     * returns the current circbuf size
     */
    int size() const
    {
        return m_buf_size;
    };

  private:
    std::vector<T> m_buf;
    int m_buf_size; // semi constant size that can't be changed but with a call to .resize; indexed at 1
    int m_occupation;
    bool m_override;
    int m_current_element_index = 0;
};
} // namespace sheep
#endif // CIRC_BUF_H
