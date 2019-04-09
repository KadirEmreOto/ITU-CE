function errors = question1()
    function y = mye(n)
        y = (1 + 1./n).^n;
    end

    errors = zeros(1,10);
    for i = 1:10,
        errors(i) = abs(exp(1.0) - mye(8^i)) / exp(1.0);
    end
    
    plot(errors);
    title('Relative Error')
    xlabel('n = 8 \^ x') 
    ylabel('Error') 
end