dTest <- read.table("./Diagonal/TestErrors",
                     sep = " ",
                     header = FALSE)
dTrain <- read.table("./Diagonal/TrainingErrors",
                      sep = " ",
                      header = FALSE)
hTest <- read.table("./Horizontal/TestErrors",
                     sep = " ",
                     header = FALSE)
hTrain <- read.table("./Horizontal/TrainingErrors",
                      sep = " ",
                      header = FALSE)

plot(x = dTest$V1,
     y = dTest$V2,
     type = "b",
     pch = 19,
     col = "red",
     log = 'x',
     ylim = c(0, 35),
     xlab = "Dimensión", 
     ylab = "Error Porcentual",
     main = "K Nearest Neighbors Por Distancia")

lines(x = dTrain$V1,
      y = dTrain$V2,
      pch = 18, 
      col = "red", 
      type = "b", 
      lty = 2)

lines(x = hTest$V1,
      y = hTest$V2,
      pch = 19, 
      col = "blue", 
      type = "b")

lines(x = hTrain$V1,
      y = hTrain$V2,
      pch = 18, 
      col = "blue", 
      type = "b", 
      lty = 2)

legend("topleft", legend=c("Test Error Diagonal", "Training Error Diagonal", "Test Error Paralelo", "Training Error Paralelo"),
       col=c("red", "red", "blue", "blue"), lty = 1:2, cex=0.8)
