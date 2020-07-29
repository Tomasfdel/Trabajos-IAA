library(comprehenr)
library(ggplot2)

values <- read.table("./0.mse",
                     sep = "\t",
                     header = FALSE)

df <- data.frame(Tipo=rep(c("Entrenamiento", "Test"), each=100000/200),
                 epoch=rep(to_vec(for(i in 1:100000) if(i %% 200==0) i), 2),
                 mse=c(values$V2, values$V4))

ggplot(data=df, aes(x=epoch, y=mse, group=Tipo)) +
  geom_line(aes(color=Tipo)) +
  geom_point(aes(color=Tipo)) +
  labs(title="Gamma = 1e-4",x="Épocas de Entrenamiento", y = "MSE") +
  theme(plot.title = element_text(hjust = 0.5, face = "bold"))
