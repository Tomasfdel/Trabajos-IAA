train <- read.table("./Diagonal/2/TrainingErrors",
                    sep = " ",
                    header = FALSE)
valid <- read.table("./Diagonal/2/ValidationErrors",
                     sep = " ",
                     header = FALSE)
test <- read.table("./Diagonal/2/TestErrors",
                     sep = " ",
                     header = FALSE)

plot(x = train$V1,
     y = train$V2,
     type = "b",
     pch = 19,
     col = "red",
     xlim = c(0, 100),
     ylim = c(0, 60),
     xlab = "K", 
     ylab = "Error Porcentual",
     main = "")

lines(x = valid$V1,
      y = valid$V2,
      pch = 19, 
      col = "blue", 
      type = "b")

lines(x = test$V1,
      y = test$V2,
      pch = 19, 
      col = "black", 
      type = "b")

legend("topright", legend=c("Training Error", "Validation Error", "Test Error"),
       col=c("red", "blue", "black"), lty = 1, cex=0.8)
