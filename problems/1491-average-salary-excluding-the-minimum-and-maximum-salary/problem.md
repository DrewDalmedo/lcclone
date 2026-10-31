id: 1491
slug: average-salary-excluding-the-minimum-and-maximum-salary
title: Average Salary Excluding the Minimum and Maximum Salary
difficulty: easy
tags: array, sorting
signature: double average(int[] salary)
---
`salary[i]` is the pay of employee `i`, and no two employees are paid the
same. Set aside the single lowest salary and the single highest salary, and
return the average of the `salary.length - 2` salaries that are left.

The average need not be a whole number. An answer within `10^-5` of the
exact average is accepted.

@examples

## Constraints

- `3 <= salary.length <= 100`
- `1000 <= salary[i] <= 10^6`
- All the values of `salary` are different.
