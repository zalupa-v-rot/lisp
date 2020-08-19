(let ((name (quote +)))
  (let ((+ (quote *)))
    (eval (list name 2 3))))
