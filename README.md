dg-mru
======

# MRU Cache
This is thread safe MRU cache implementation. It locks insert and retrieval operations with a simple reader writer lock.
Because the cache is updated even on retrieve operations the lock is acquired exclusively every time.

### API description: ###



1. **Function:**	*insert*

	**Purpose:**  
		Inserts an element into MRU cache. If an element with same key
		exists old value is replaced by new value. If MRU cache exceeds max size
		oldest accessed element is removed.
	
	**Parameters:**

  		const K& key  - key of the element
		V& value      - value of new element.  A copy of value is created upon insertion
	
	**Returns:**  void


2. **Function:** at
	
	**Purpose:**  
	    Retrieves an element from MRU cache. Accessing an element updates 
	    its order for cache removal

	**Parameters:**

		const K& key  - key of the element

	**Returns:** 
	
		std::shared_ptr<V> - value of the requested element. If an element with a requested key
	                         is not in the cache nullptr is returned. Note that the returned type
	                         is a shared_ptr and not a copy of the element.