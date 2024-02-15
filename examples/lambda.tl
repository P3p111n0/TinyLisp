(defun use_lambda (proc e)
	(proc e)
)

(use_lambda (lambda (x) (x 2 *)) 2)
