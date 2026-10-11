;;; -*- lexical-binding: t -*-

(defconst ochre-mode-syntax-table
  (with-syntax-table (copy-syntax-table)
    ;; Chars are the same as strings
    (modify-syntax-entry ?' "\"")
    (syntax-table)))

(defconst ochre-keywords
  '("fun" "struct" "const" "end" "import" ))

(defconst ochre-funcs '("cast" "delete" "call0" "call1" "call2" "call3" "call4" "call5" "call6" "dump"))

(defconst ochre-builtin '("jmp" "cjmpt" "cjmpf" "jmpe"  "cjmpet" "cjmpef"))

(defconst ochre-types
  '("Int" "Char"))

(defconst ochre-highlights `(
  ("#.*" . font-lock-comment-face)
  ("-?\\<-?[0-9]+\\(\\.[0-9]+\\)?\\>"     . font-lock-constant-face)
  ("\\_<&\\S-+"                           . font-lock-variable-name-face)
  ("\\(?:^\\|\\s-\\)@\\S-+"               . font-lock-variable-name-face)
  (,(regexp-opt ochre-keywords 'symbols)  . font-lock-keyword-face)
  (,(regexp-opt ochre-funcs 'symbols)     . font-lock-function-name-face)
  (,(regexp-opt ochre-builtin 'symbols)   . font-lock-builtin-face)
  (,(regexp-opt ochre-types 'symbols)     . font-lock-type-face)))

(define-derived-mode ochre-mode prog-mode "Ochre"
  "Major Mode for editing Ochre source code."
  :syntax-table ochre-mode-syntax-table
  (setq font-lock-defaults '(ochre-highlights)))

(add-to-list 'auto-mode-alist '("\\.och\\'" . ochre-mode))
