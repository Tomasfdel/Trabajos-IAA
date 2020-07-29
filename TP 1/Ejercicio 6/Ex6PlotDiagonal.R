dPre <- read.table("./diagonal_Before-Prunning",
                    sep = " ",
                    header = FALSE)
dPost <- read.table("./diagonal_After-Prunning",
                     sep = " ",
                     header = FALSE)
bayes <- read.table("./diagonal_Bayes",
                     sep = " ",
                     header = FALSE)

plot(x = dPre$V1,
     y = dPre$V2,
     type = "b",
     pch = 19,
     col = "red",
     xlim = c(0.4, 2.6),
     ylim = c(0, 50),
     xlab = "C", 
     ylab = "Test Error Porcentual",
     main = "Diagonal")

lines(x = dPost$V1,
      y = dPost$V2,
      pch = 18, 
      col = "blue", 
      type = "b", 
      lty = 2)

lines(x = bayes$V1,
      y = bayes$V2,
      pch = 18, 
      col = "black", 
      type = "b")

legend("topright", legend=c("Before-Prunning", "After-Prunning", "Bayes"),
       col=c("red", "blue", "black"), lty = 1:2, cex=0.8)
