train <- read.table("./spirals_TrainingErrors",
                    sep = " ",
                    header = FALSE)
valid <- read.table("./spirals_ValidationErrors",
                     sep = " ",
                     header = FALSE)
test <- read.table("./spirals_TestErrors",
                     sep = " ",
                     header = FALSE)

plot(x = train$V1,
     y = train$V2,
     type = "b",
     pch = 19,
     col = "red",
     xlim = c(0, 201),
     ylim = c(0, 60),
     xlab = "Cantidad de Bins", 
     ylab = "Error Porcentual",
     main = "Espirales Anidadas")

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
