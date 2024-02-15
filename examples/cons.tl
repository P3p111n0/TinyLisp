; idk what to show here, comparison with null and quoting is not yet implemented, so :( 

(defun cadr (lst)
	(car (cdr lst))
)

(cadr (cons 1 (cons 2 null)))
