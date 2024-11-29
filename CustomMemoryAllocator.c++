#include <iostream>
using namespace std;

// Structure to represent memory blocks
struct MemoryBlock {
    int start;          // Starting address
    int size;           // Size of the block
    bool isFree;        // Free or allocated
    MemoryBlock* next;  // Pointer to the next memory block

    // Constructor to initialize the memory block
    MemoryBlock(int startAddr, int blockSize, bool free = true) {
        start = startAddr;
        size = blockSize;
        isFree = free;
        next = nullptr;
    }
};

// Class to manage memory allocation
class MemoryAllocator {
private:
    MemoryBlock* head; // Pointer to the first memory block

public:
    // Constructor to initialize memory with a single block
    MemoryAllocator(int totalSize) {
        head = new MemoryBlock(0, totalSize); // Entire memory is free initially
    }

    // Function to allocate memory
    int allocate(int size) {
        MemoryBlock* current = head;

        while (current) {
            if (current->isFree && current->size >= size) {
                // Split the block if the size is larger than requested
                if (current->size > size) {
                    MemoryBlock* newBlock = new MemoryBlock(current->start + size, current->size - size);
                    newBlock->next = current->next;
                    current->next = newBlock;
                    current->size = size;
                }
                current->isFree = false;
                cout << "Allocated " << size << " bytes at address " << current->start << "." << endl;
                return current->start;
            }
            current = current->next;
        }

        cout << "Memory allocation failed! Not enough space." << endl;
        return -1;
    }

    // Function to free memory
    void free(int startAddr) {
        MemoryBlock* current = head;

        while (current) {
            if (current->start == startAddr && !current->isFree) {
                current->isFree = true;
                cout << "Memory at address " << startAddr << " has been freed." << endl;
                mergeFreeBlocks();
                return;
            }
            current = current->next;
        }

        cout << "Invalid free request! Address not found." << endl;
    }

    // Merge adjacent free blocks
    void mergeFreeBlocks() {
        MemoryBlock* current = head;

        while (current && current->next) {
            if (current->isFree && current->next->isFree) {
                current->size += current->next->size;
                MemoryBlock* toDelete = current->next;
                current->next = current->next->next;
                delete toDelete;
            } else {
                current = current->next;
            }
        }
    }

    // Function to display memory layout
    void displayMemory() {
        MemoryBlock* current = head;

        cout << "\nMemory Layout:" << endl;
        while (current) {
            cout << "Start: " << current->start << ", Size: " << current->size 
                 << ", Status: " << (current->isFree ? "Free" : "Allocated") << endl;
            current = current->next;
        }
    }
};

// Main function
int main() {
    MemoryAllocator allocator(100); // Initialize memory of size 100 bytes

    allocator.allocate(30);
    allocator.allocate(20);
    allocator.displayMemory();
    allocator.free(30);
    allocator.displayMemory();
    allocator.allocate(40);
    allocator.displayMemory();

    return 0;
}
