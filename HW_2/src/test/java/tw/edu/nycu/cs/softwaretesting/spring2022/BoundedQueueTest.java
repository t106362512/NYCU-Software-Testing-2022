package tw.edu.nycu.cs.softwaretesting.spring2022;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

// C1	Constructor
// C2	If argument is less than 0
// C3	Make o the newest element of the queue
// C4	If argument is null
// C5	Remove and return oldest element of the queue
// C6	If queue is empty
// C7	If queue is full

class BoundedQueueTest {

    private BoundedQueue bq;

    @BeforeEach
    void setUp() {
        bq = new BoundedQueue(2);
    }

    @AfterEach
    void tearDown() {
        bq = null;
    }

    // C1=True, C2=False
    @Test
    void boundedQueue_BaseCase() {
        assertTrue(BoundedQueue.class.isInstance(bq));
    }

    // C1=True, C2=True
    @Test
    void boundedQueue_C2() throws IllegalArgumentException {
        IllegalArgumentException e = assertThrows(IllegalArgumentException.class, () -> {
            new BoundedQueue(-1);
        });
        assertEquals("BoundedQueue.constructor", e.getMessage());
    }

    // C3=True, C4=False, C7=False
    @Test
    void enQueue_BaseCase() {
        bq.enQueue('C');
        assertEquals("[C]", bq.toString());
    }

    // C3=True, C4=True, C7=False
    @Test
    void enQueue_C4() throws NullPointerException {
        NullPointerException e = assertThrows(NullPointerException.class, () -> {
            bq.enQueue(null);
        });
        assertEquals("BoundedQueue.enQueue", e.getMessage());
    }

    // C3=True, C4=False, C7=True
    @Test
    void enQueue_C7() {
        bq.enQueue('a');
        bq.enQueue('b');
        IllegalStateException e = assertThrows(IllegalStateException.class, () -> {
            bq.enQueue('c');
        });
        assertEquals("BoundedQueue.enQueue", e.getMessage());
    }

    // C5=True, C6=False
    @Test
    void deQueue_BaseCase() {
        bq.enQueue(1);
        assertEquals(1, bq.deQueue());
    }

    // C5=True, C6=True
    @Test
    void deQueue_C6() throws IllegalStateException {
        IllegalStateException e = assertThrows(IllegalStateException.class, () -> {
            bq.deQueue();
        });
        assertEquals("BoundedQueue.deQueue", e.getMessage());
    }

    // C6=True
    @Test
    void isEmpty_BaseCase() {
        assertTrue(bq.isEmpty());
    }

    // C6=False
    @Test
    void isEmpty_C6() {
        bq.enQueue(1);
        assertFalse(bq.isEmpty());
    }

    // C7=True
    @Test
    void isFull_BaseCase() {
        bq.enQueue(1);
        bq.enQueue(2);
        assertTrue(bq.isFull());
    }

    // C7=False
    @Test
    void isFull_C7() {
        assertFalse(bq.isFull());
    }
}