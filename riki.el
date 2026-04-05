;;; -*- lexical-binding: t -*-

(defconst riki-mode-syntax-table
  (with-syntax-table (copy-syntax-table)
    ;; Chars are the same as strings
    (modify-syntax-entry ?' "\"")
    (syntax-table)))

(defconst riki-builtin-value
  '("true" "false" "none"))

(defconst riki-keywords
  '("if" "else" "while" "use" "fn"
    "type" "expr" "extern" "as"))

(defconst riki-builtin '("call" "unwrap"))

(defconst riki-types
  '("i64" "i32" "u64" "u32" "u8" "f64" "f32" "bool" "string" "list"))

(defconst riki-highlights `(
  ("#.*" . font-lock-comment-face)
  ("-?\\<-?[0-9]+\\(\\.[0-9]+\\)?\\>"        . font-lock-constant-face)
  (,(regexp-opt riki-builtin-value 'symbols) . font-lock-constant-face)
  (,(regexp-opt riki-keywords 'symbols)      . font-lock-keyword-face)
  (,(regexp-opt riki-builtin 'symbols)       . font-lock-builtin-face)
  (,(regexp-opt riki-types 'symbols)         . font-lock-type-face)))

(define-derived-mode riki-mode prog-mode "Riki"
  "Major Mode for editing Riki source code."
  :syntax-table riki-mode-syntax-table
  (setq font-lock-defaults '(riki-highlights)))

(add-to-list 'auto-mode-alist '("\\.ri\\'" . riki-mode))
