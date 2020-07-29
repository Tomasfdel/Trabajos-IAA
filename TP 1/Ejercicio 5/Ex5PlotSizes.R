dSize <- read.table("./diagonal_TreeSizes",
                     sep = " ",
                     header = FALSE)
hSize <- read.table("./horizontal_TreeSizes",
                     sep = " ",
                     header = FALSE)

plot(x = dSize$V1,
     y = dSize$V2,
     type = "b",
     pch = 19,
     col = "red",
     log = 'x',
     ylim = c(0, 25),
     xlab = "Tamaño de Muestra de Entrenamiento", 
     ylab = "Nodos del Árbol",
     main = "")

lines(x = hSize$V1,
      y = hSize$V2,
      pch = 19, 
      col = "blue", 
      type = "b")

legend("topleft", legend=c("Diagonal", "Paralelo"),
       col=c("red", "blue"), lty = 1:1, cex=0.8)
