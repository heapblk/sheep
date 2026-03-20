#pragma once
#include <vector>

namespace sheep
{
/*
 * @brief faster circular buffer implementation using a dynamic array
 */
template <class T> class CircBuf
{
public:
    /*
     * @brief Size constructor
     * 
     * @param size initial size of the buffer
     */
    explicit CircBuf(const int size)
    {
        m_buf.resize(size);
    };

    /*
     * @brief Vector constructor
     * 
     * @param vec vector which contains data 
     * NOTE: it is assumed that the whole vector is filled with data that's going to be used
     */
    explicit CircBuf(const std::vector<T> &vec) : m_occupation(vec.size())
    {
        m_buf = vec;
    }

    /*
     * @brief Destructor
     */
    ~CircBuf() = default;

    /*
     * @brief appends one element to the circular buffer
     * 
     * @param element element to append
     * 
     * @return returns false once the buffer is full and doesn't append the given element, otherwise returns true and
     *         appends the given element except if the override is set, then it'll always return true and override elements
     */
    bool push_back(T element)
    {
        bool _success = false;
        // there is no space anymore
        if (m_occupation >= m_buf.size())
        {
            // override flag is set, so start overwriting
            if (m_override || m_clear)
            {
                this->at(m_current_element_index) = element;
                m_current_element_index++;

                // once we reach the end of the buffer,
                // we'll have to unset the clear flag to avoid
                // overriding the first elements
                if (m_current_element_index == m_buf.size())
                {
                    m_clear = false;
                }
                _success = true;
            }
            else
            {
                _success = false;
            }
        }
        else
        {
            // there is still space for a new element
            m_buf.at(m_occupation) = element;
            m_occupation++;
            _success = true;
        }
        return _success;
    };

    /*
     * @brief returns the element at the current index
     * 
     * @param index
     * NOTE: the index wraps around
     * 
     * @return element at given index
     */
    T &at(int index)
    {
        while (index >= m_buf.size())
        {
            index = m_buf.size() % index;
        }
        return m_buf.at(index);
    }

    /*
     * @brief sets whether push_back should simply override the elements when the buffer is full
     * 
     * @param override true if override should be enabled, otherwise false
     */
    void set_override(bool override)
    {
        m_override = override;
    };

    /*
     * @brief clears internal data array
     */
    /// IMPL: sort of a pseudo clear, simply releases all objects so they can be overwritten
    void clear()
    {
        m_clear = true;
        m_occupation = m_buf.size();
    };

    /*
     * @brief allocate memory based on new size
     * 
     * @param size amount of new memory cells to allocat
     */
    void resize(const int size)
    {
        m_buf.resize(size);
    };

    /*
     * @brief returns the current circbuf size
     * 
     * @return current size
     */
    [[nodiscard]]
    int size() const
    {
        return m_buf.size();
    };

private:
    std::vector<T> m_buf; // internal buffer
    int m_occupation = 0; // occupied memory cells in buffer
    bool m_override = false; // override flag
    int m_current_element_index = 0; // current element index
    bool m_clear = false; // clear flag
};
} // namespace sheep