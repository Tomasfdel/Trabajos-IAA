train <- read.table("./Results/TrainingErrors",
                    sep = " ",
                    header = FALSE)
valid <- read.table("./Results/ValidationErrors",
                     sep = " ",
                     header = FALSE)
test <- read.table("./Results/TestErrors",
                     sep = " ",
                     header = FALSE)

plot(x = train$V1,
     y = train$V2,
     type = "b",
     pch = 19,
     col = "red",
     xlim = c(0, 50),
     ylim = c(0.01, 0.03),
     xlab = "K", 
     ylab = "MSE",
     main = "Sunspots")

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

legend("topleft", legend=c("Training Error", "Validation Error", "Test Error"),
       col=c("red", "blue", "black"), lty = 1, cex=0.8)
