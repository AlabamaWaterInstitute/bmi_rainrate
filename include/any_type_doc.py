# Documentation / Design file for any_type.hpp

# AnyType is a c++ class intended to store any type of data, and allow
# one variable to store multiple different types of data.
# In the bmi module, we use it to store the data for the different model
# variables, which can be of different types.

# We do this without the use of full-class templates, which would require
# each type to be defined at compile time. Instead, we use a void pointer
# and a type identifier to store the data, and store a type-specific function
# pointer to retrieve the data.

class void_pointer:
    pass

class function_pointer:
    pass

class AnyType:
    _type: str = None
    _data: void_pointer = None
    # Not final, but this basic pattern will be used
    # i.e. for each type-reliant function, there will be a function pointer
    get_val: function_pointer = None
    set_val: function_pointer = None
    destroy: function_pointer = None

    """
    class AnyType {
    public:
        template <typename T>
        AnyType(T data);
        ~AnyType();
        void* _data;
        std::string _type;
        void* (*get_val)();
        void (*set_val)();
        void (*_destroy)();
    
    """

    # Constructor
    # The constructor will be a template that uses the type of the input
    # to set up the type identifier and the function pointer.
    # The input will be stored in the void pointer.

    """
    template <typename T>
    AnyType(T data) {
        _type = typeid(T).name();
        _data = new T(data);
        get_val = &_get<T>;
        set_val = &_set<T>;
        destroy = &destroy<T>;
    }
    """
    def __init__(self, data):
        self._type = type(data).__name__
        self._data = data
        self.get_val = self._get
        self.set_val = self._set
        self.destroy = self._destroy


    # Destructor
    # The destructor will re-cast the void pointer to the original type
    # and delete it.

    """
    ~AnyType() {
        _destroy(_data);
    }
    """
    def __del__(self):
        self.destroy(self._data)

    # _get
    # This function will re-cast the void pointer to the original type
    # and return it.

    """
    template <typename T>
    T _get() {
        return *static_cast<T*>(_data);
    }
    """
    def _get(self):
        return self._data
    
    # _set
    # This function will overwrite the data in the void pointer with the
    # input data.

    """
    template <typename T>
    void _set(T data) {
        *static_cast<T*>(_data) = data;
    }
    """
    def _set(self, data):
        self._data = data

    # destroy
    # This function will delete the void pointer.

    """
    template <typename T>
    void _destroy(void* data) {
        delete static_cast<T*>(data);
    }
    """
    def _destroy(self, data):
        del data

