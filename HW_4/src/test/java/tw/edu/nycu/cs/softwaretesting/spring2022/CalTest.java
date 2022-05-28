package tw.edu.nycu.cs.softwaretesting.spring2022;

import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class CalTest {

    @Test
    public void testNormalYear() {
        assertEquals(364, Cal.cal(1, 1, 12, 31, 2022));
    }

    @Test
    public void testLeapYear() {
        assertEquals(365, Cal.cal(1, 1, 12, 31, 2024));
    }

    @Test
    public void testSameMonth() {
        assertEquals(30, Cal.cal(1, 1, 1, 31, 2022));
    }

    @Test
    public void testBiggerMonth() {
        assertEquals(31, Cal.cal(8, 1, 7, 1, 2020));
    }

    @Test
    public void testZeroDay() {
        assertEquals(0, Cal.cal(1, 1, 1, 1, 2022));
    }

    // @Test
    // public void testPOSIXStart() {
    //     assertEquals(89, Cal.cal(1, 1, 3, 31, 1970));
    // }

    // @Test
    // public void testYear2038() {
    //     assertEquals(1, Cal.cal(1, 19, 1, 20, 2038));
    // }

    // @Test
    // public void testM4Eq0AndM100Eq0AndM400Eq0() {
    //     assertEquals(29, Cal.cal(2, 1, 3, 1, 2000));
    // }

    // @Test
    // public void testM4Lt0AndM100Lt0AndM400Lt0() {
    //     assertEquals(28, Cal.cal(2, 1, 3, 1, -1));
    // }

    // @Test
    // public void testM4Eq0AndM100Lt0AndM400Lt0() {
    //     assertEquals(29, Cal.cal(2, 1, 3, 1, -4));
    // }

    // @Test
    // public void testM4Eq0AndM100Eq0AndM400Lt0() {
    //     assertEquals(28, Cal.cal(2, 1, 3, 1, -500));
    // }
}